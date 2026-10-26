#include <stdio.h>
#include <unistd.h>

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "../src/page_replacement.h"

static GtkWidget *pages_entry;
static GtkWidget *frames_entry;
static GtkWidget *algorithm_combo;
static GtkWidget *results_view;

static void run_simulation(GtkButton *button, gpointer data)
{
    (void)button;
    (void)data;

    const char *page_text =
        gtk_entry_get_text(GTK_ENTRY(pages_entry));
    const char *frame_text =
        gtk_entry_get_text(GTK_ENTRY(frames_entry));

    int pages[MAX_PAGES];
    int n = 0;
    int frames = atoi(frame_text);

    char *copy = g_strdup(page_text);
    char *token = strtok(copy, " ,\t\n");

    while (token != NULL && n < MAX_PAGES) {
        char *end;
        long value = strtol(token, &end, 10);

        if (*end != '\0' || value < 0) {
            GtkWidget *dialog = gtk_message_dialog_new(
                NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
                GTK_BUTTONS_OK, "Invalid page reference string.");
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            g_free(copy);
            return;
        }

        pages[n++] = (int)value;
        token = strtok(NULL, " ,\t\n");
    }

    g_free(copy);

    if (n == 0 || frames < 1 || frames > MAX_FRAMES) {
        GtkWidget *dialog = gtk_message_dialog_new(
            NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Enter at least one page and 1-%d frames.",
            MAX_FRAMES);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }

    GtkTextBuffer *buffer =
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(results_view));
    gtk_text_buffer_set_text(buffer, "", -1);

    GtkTextIter iter;
    gtk_text_buffer_get_end_iter(buffer, &iter);

    Result result;
    const char *algorithm =
        gtk_combo_box_text_get_active_text(
            GTK_COMBO_BOX_TEXT(algorithm_combo));

    if (algorithm == NULL)
        return;

    char output[4096];
    int old_stdout = dup(STDOUT_FILENO);
    FILE *temp = tmpfile();

    if (temp == NULL) {
        g_free((gpointer)algorithm);
        close(old_stdout);
        return;
    }

    fflush(stdout);
    dup2(fileno(temp), STDOUT_FILENO);

    if (strcmp(algorithm, "FIFO") == 0)
        fifo(pages, n, frames, &result);
    else if (strcmp(algorithm, "LRU") == 0)
        lru(pages, n, frames, &result);
    else if (strcmp(algorithm, "Optimal") == 0)
        optimal(pages, n, frames, &result);
    else {
        Result results[3];
        fifo(pages, n, frames, &results[0]);
        lru(pages, n, frames, &results[1]);
        optimal(pages, n, frames, &results[2]);
    }

    fflush(stdout);
    long length = ftell(temp);
    rewind(temp);

    if (length >= (long)sizeof(output))
        length = sizeof(output) - 1;

    size_t bytes = fread(output, 1, (size_t)length, temp);
    output[bytes] = '\0';

    dup2(old_stdout, STDOUT_FILENO);
    close(old_stdout);
    fclose(temp);

    gtk_text_buffer_set_text(buffer, output, -1);
    g_free((gpointer)algorithm);
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),
                         "Page Replacement Simulator");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);
    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(box), 15);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *title = gtk_label_new(
        "PAGE REPLACEMENT ALGORITHM SIMULATOR");
    gtk_box_pack_start(GTK_BOX(box), title, FALSE, FALSE, 5);

    pages_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(
        GTK_ENTRY(pages_entry), "Example: 7 0 1 2 0 3 0 4 2 3");
    gtk_box_pack_start(GTK_BOX(box), pages_entry,
                       FALSE, FALSE, 5);

    frames_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(frames_entry),
                                   "Number of frames (1-20)");
    gtk_box_pack_start(GTK_BOX(box), frames_entry,
                       FALSE, FALSE, 5);

    algorithm_combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo), "FIFO");
    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo), "LRU");
    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo), "Optimal");
    gtk_combo_box_text_append_text(
        GTK_COMBO_BOX_TEXT(algorithm_combo), "Compare all");
    gtk_combo_box_set_active(GTK_COMBO_BOX(algorithm_combo), 3);
    gtk_box_pack_start(GTK_BOX(box), algorithm_combo,
                       FALSE, FALSE, 5);

    GtkWidget *button = gtk_button_new_with_label("Run Simulation");
    g_signal_connect(button, "clicked",
                     G_CALLBACK(run_simulation), NULL);
    gtk_box_pack_start(GTK_BOX(box), button,
                       FALSE, FALSE, 5);

    results_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(results_view), FALSE);
    gtk_text_view_set_monospace(GTK_TEXT_VIEW(results_view), TRUE);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scroll), results_view);
    gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, 5);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
