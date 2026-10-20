#ifndef STATE_H
#define STATE_H
#define MAX_P 30
#define MAX_R 10
typedef struct {
    int p, r;
    char process[MAX_P][16];
    char resource[MAX_R][16];
    int total[MAX_R];
    int allocation[MAX_P][MAX_R];
    int maximum[MAX_P][MAX_R];
    int request[MAX_P][MAX_R];
    int active[MAX_P];
} SystemState;
void init_sample(SystemState *s);
int available(SystemState *s, int out[MAX_R]);
int validate(SystemState *s, char *message, int message_size);
void print_matrices(SystemState *s);
#endif
