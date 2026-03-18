#include <complex.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"

#define SPACE ' '
#define LOGIC_EXPRESSION "01&|!"
#define SINGLE_CHARACTER "01!"
#define CONJUNCTION '*'
#define DISJUNCTION '+'

status Input(Stack *stack, Stack *res, const char *argv, bool *new_expression) {
    const char *ptr = NULL;
    status x = OK;
    for (ptr = argv; ptr < argv + strlen(argv); ptr++) {
        x = StackPush(stack, *ptr);
        if (x != OK) {
            return x;
        }
        x = StackOnlyLogicPush(res, *ptr, new_expression);
        if (x != OK) {
            return x;
        }
    }
    return OK;
}

status Task(Stack *stack, char *result) {
    if (!stack) {
        return false;
    }
    char cur_value, val1 = 0, val2 = 0, local_res = 0, is_expr = 0;
    Stack *operations_stack = StackCreate();
    if (!operations_stack) {
        return MEMORY_ERROR;
    }
    while (!IsEmpty(stack)) {
        cur_value = StackPop(stack);
        SpaceSkip(stack);
        if (IsNumber(cur_value)) {
            StackPush(operations_stack, cur_value);
        } else {
            switch (cur_value) {
                case '!':
                    val1 = StackPop(operations_stack);
                    if (val1 < (char)'0') {
                        StackFree(operations_stack);
                        return WRONG_EXPRESSION;
                    }
                    local_res = ((val1 == '0') ? '1' : '0');
                    break;
                case '*':
                case '+':
                    val2 = StackPop(operations_stack);
                    if (val2 < (char)'0') {
                        StackFree(operations_stack);
                        return WRONG_EXPRESSION;
                    }
                    val1 = StackPop(operations_stack);
                    if (val1 < (char)'0') {
                        StackFree(operations_stack);
                        return WRONG_EXPRESSION;
                    }
                    if (cur_value == (char)'*') {
                        local_res = ((val1 == '1' && val2 == '1') ? '1' : '0');
                    } else {
                        local_res = ((val1 == '1' || val2 == '1') ? '1' : '0');
                    }
            }
            StackPush(operations_stack, local_res);
            is_expr = 1;
        }
    }
    if (!is_expr) {
        StackFree(operations_stack);
        return WRONG_EXPRESSION;
    }
    *result = StackPop(operations_stack);
    if (StackPop(operations_stack) != STACK_EMPTY) {
        StackFree(operations_stack);
        return WRONG_EXPRESSION;
    }
    StackFree(operations_stack);
    return OK;
}
    
status StackOnlyLogicPush(Stack *stack, const char value, bool *new_expression) {
    if (!stack) {
        return NULLPTR;
    }
    status x = OK;
    static char prev_value = 0, counter_conj = 0, counter_disj = 0;
    bool first_space = true;
    if (*new_expression) {
        prev_value = 0;
        counter_conj = 0;
        counter_disj = 0;
        *new_expression = false;
    }
    if (prev_value == SPACE) {
        first_space = false;
    }
    if (strchr(LOGIC_EXPRESSION, (int)value)) {
            if (strchr(SINGLE_CHARACTER, (int)value)) {
                x = StackPush(stack, value);
                if (x != OK) {
                    return x;
                }
            }
            if (value == (char)'&') {
                if (counter_conj == 1) { 
                    x = StackPush(stack, (char)CONJUNCTION);
                    counter_conj = 0;
                    if (x != OK) {
                        return x;
                    }
                } else {
                    counter_conj++;
                }
            }
            if (value == (char)'|') {
                if (counter_disj == 1) {
                    StackPush(stack, (char)DISJUNCTION);
                    counter_disj = 0;
                    if (x != OK) {
                        return x;
                    }
                } else {
                    counter_disj++;
                }
            }
            prev_value = value;
    } else {
        if (value == SPACE && first_space) {
            if (counter_disj == 1 || counter_conj == 1) {
                prev_value = value;
                return OK;
            }
            StackPush(stack, SPACE);
            prev_value = value;
        } else {
            return WRONG_EXPRESSION;
        }
    }
    return OK;
}

bool IsNumber(char value) {
    return (value == (char)'0' || value == (char)'1');
}

void SpaceSkip(Stack *stack) {
    StackPop(stack);
}
