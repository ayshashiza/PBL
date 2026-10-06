/*
 * Standalone reference benchmark for the Week 2 simulator.
 * Models the verified behavior: load 10, 20, 30, 40; run to ACC=100, PC=4.
 * This is a separate reference implementation, not the original Week 1 program.
 */
#define _POSIX_C_SOURCE 199309L

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define VALUE_COUNT 4
#define ITERATIONS 1000000ULL

typedef struct {
    int memory[VALUE_COUNT];
    int acc;
    int pc;
} Machine;

static void load_values(Machine *machine) {
    machine->memory[0] = 10;
    machine->memory[1] = 20;
    machine->memory[2] = 30;
    machine->memory[3] = 40;
}

static void run_program(Machine *machine) {
    machine->acc = 0;
    machine->pc = 0;

    while (machine->pc < VALUE_COUNT) {
        machine->acc += machine->memory[machine->pc];
        machine->pc++;
    }
}

static double seconds_between(struct timespec start, struct timespec end) {
    return (double)(end.tv_sec - start.tv_sec) +
           (double)(end.tv_nsec - start.tv_nsec) / 1000000000.0;
}

int main(void) {
    Machine machine = {0};
    struct timespec start, end;
    volatile int result_guard = 0;

    load_values(&machine);
    run_program(&machine);

    if (machine.acc != 100 || machine.pc != 4) {
        fprintf(stderr, "Reference behavior check failed: ACC=%d PC=%d\n",
                machine.acc, machine.pc);
        return 1;
    }

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (uint64_t i = 0; i < ITERATIONS; i++) {
        load_values(&machine);
        machine.acc = 0;
        machine.pc = 0;
        run_program(&machine);
        result_guard += machine.acc + machine.pc;
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    printf("Mode: standalone reference\n");
    printf("Memory: %d %d %d %d\n",
           machine.memory[0], machine.memory[1],
           machine.memory[2], machine.memory[3]);
    printf("Registers: ACC=%d PC=%d\n", machine.acc, machine.pc);
    printf("Iterations: %llu\n", (unsigned long long)ITERATIONS);
    printf("Elapsed seconds: %.6f\n", seconds_between(start, end));
    printf("Result check: %d\n", result_guard);
    return 0;
}
