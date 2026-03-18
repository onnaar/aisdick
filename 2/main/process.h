#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include "stack.h"

status Input(Stack *, Stack *, const char *, bool *);
bool IsNumber(const char);
status Task(Stack *, char *);
status StackOnlyLogicPush(Stack *, const char, bool *);
void SpaceSkip(Stack *);

#endif
