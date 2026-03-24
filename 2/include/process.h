#ifndef PROCESS_H
#define PROCESS_H

#include "stack.h"

status Processing(const char *const, Stack **, Stack **, size_t);
status Input(Stack *const, Stack *const, const char *const, bool *const, bool *const);
bool IsNumber(const char);
status Task(Stack *const, char *const, size_t);
status StackOnlyLogicPush(Stack *const, const char, bool *const);
void SpaceSkip(Stack *const);

#endif
