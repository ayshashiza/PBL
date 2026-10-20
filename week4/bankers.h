#ifndef BANKERS_H
#define BANKERS_H
#include "state.h"
int safe_sequence(SystemState *s, int seq[MAX_P], int *seq_len);
int request_resources(SystemState *s, int pi, int req[MAX_R], char *msg, int msg_size);
#endif
