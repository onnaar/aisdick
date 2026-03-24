#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "stack.h"

#define SPACE ' '
#define LOGIC_EXPRESSION "01&|!"
#define SINGLE_CHARACTER "01!"
#define CONJUNCTION '*'
#define DISJUNCTION '+'

status Processing(const char *const argv, Stack **input_stack, Stack **only_logic_stack, size_t size) {
    if (!argv) {
        return NULLPTR;
    }
    char result = '1';
    bool new_expression = true;
    status x = OK;
    *input_stack = StackCreate(sizeof(char) * strlen(argv));
    if (!input_stack) {
        return MEMORY_ERROR;
    }
    *only_logic_stack = StackCreate(size);
    if (!only_logic_stack) {
        return MEMORY_ERROR;
    }
    bool is_right_expression = true;
    x = Input(*input_stack, *only_logic_stack, argv, &new_expression, &is_right_expression);
    if (x != OK) {
        return x;
    }
    printf("all-symbol stack: ");
    StackOutput(*input_stack);
    printf("logical-symbol stack: ");
    StackFakeOutput(*only_logic_stack);
    printf("\n");
    x = Task(*only_logic_stack, &result, size);
    if (x != OK) {
        return x;
    }
    if (is_right_expression) {
        printf("result: %c\n\n", result);
    } else {
        return WRONG_EXPRESSION;
    }
    return OK;
}
    
status Input(Stack *const input_stack, Stack *const only_logic_stack, const char *argv, bool *const new_expression, bool *const is_right_expression) {
    const char *ptr = NULL;
    status x = OK;
    for (ptr = argv; ptr < argv + strlen(argv); ptr++) {
        x = StackPush(input_stack, *ptr);
        if (x != OK) {
            return x;
        }
        x = StackOnlyLogicPush(only_logic_stack, *ptr, new_expression);
        if (x != OK) {
            *is_right_expression = false;
            if (x == STACK_OVERFLOW) {
                return x;
            }
        }
    }
    return OK;
}

status Task(Stack *const only_logic_stack, char *const result, size_t size) {
    if (!only_logic_stack) {
        return false;
    }
    
    char cur_value, val1 = 0, val2 = 0, local_res = 0, is_expr = 0;
    Stack *operations_stack = StackCreate(size);
    if (!operations_stack) {
        return MEMORY_ERROR;
    }
    while (!IsEmpty(only_logic_stack)) {
        cur_value = StackPop(only_logic_stack);
        SpaceSkip(only_logic_stack);
        if (IsNumber(cur_value)) {
            StackPush(operations_stack, cur_value);
        } else {
            switch (cur_value) {
                case '!':
                    val1 = StackPop(operations_stack);
                    if (val1 < '0') {
                        StackFree(operations_stack);
                        return WRONG_EXPRESSION;
                    }
                    local_res = ((val1 == '0') ? '1' : '0');
                    break;
                case '*':
                case '+':
                    val2 = StackPop(operations_stack);
                    if (val2 < '0') {
                        StackFree(operations_stack);
                        return WRONG_EXPRESSION;
                    }
                    val1 = StackPop(operations_stack);
                    if (val1 < '0') {
                        StackFree(operations_stack);
                        return WRONG_EXPRESSION;
                    }
                    if (cur_value == '*') {
                        local_res = ((val1 == '1' && val2 == '1') ? '1' : '0');
                    } else {
                        local_res = ((val1 == '1' || val2 == '1') ? '1' : '0');
                    }
            }
            StackPush(operations_stack, local_res);
            is_expr = 1;
        }
        printf("logical-symbol stack: ");
        StackFakeOutput(only_logic_stack);
        printf("operations_stack: ");
        StackFakeOutput(operations_stack);
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
    
status StackOnlyLogicPush(Stack *const only_logic_stack, const char value, bool *const new_expression) {
    if (!only_logic_stack) {
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
                x = StackPush(only_logic_stack, value);
                if (x != OK) {
                    return x;
                }
            }
            if (value == '&') {
                if (counter_conj == 1) { 
                    x = StackPush(only_logic_stack, (char)CONJUNCTION);
                    counter_conj = 0;
                    if (x != OK) {
                        return x;
                    }
                } else {
                    counter_conj++;
                }
            }
            if (value == '|') {
                if (counter_disj == 1) {
                    StackPush(only_logic_stack, (char)DISJUNCTION);
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
            StackPush(only_logic_stack, SPACE);
            prev_value = value;
        } else {
            return WRONG_EXPRESSION;
        }
    }
    return OK;
}

bool IsNumber(const char value) {
    return (value == '0' || value == '1');
}

void SpaceSkip(Stack *const only_logic_stack) {
    StackPop(only_logic_stack);
}
