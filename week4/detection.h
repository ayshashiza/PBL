#ifndef DETECTION_H
#define DETECTION_H
#include "state.h"
int detect_deadlock(SystemState *s, int blocked[MAX_P]);
int terminate_process(SystemState *s,int pi,char *msg,int msg_size);
#endif
