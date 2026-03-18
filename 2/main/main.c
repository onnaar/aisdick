#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stack.h"
#include "process.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("NO PARAMETERS!!\n");
        return 0;
    }
    char *fail_name = NULL;
    Stack *stack = StackCreate();
    if (!stack) {
        fail_name = "MEMORY_ERROR";
        printf("%s\n", fail_name);
        return 0;
    }
    Stack *res = StackCreate();
    if (!res) {
        fail_name = "MEMORY_ERROR";
        printf("%s\n", fail_name);
        StackFree(stack);
        return 0;
    }
    char result = '1';
    bool new_expression = true;
    status x = OK;
    for (int i = 1; i < argc; i++) {
        new_expression = true;
        x = Input(stack, res, argv[i], &new_expression);
        if (x != OK) {
            switch (x) {
                case NULLPTR:
                    fail_name = "NULLPTR";
                    break;
                case STACK_OVERFLOW:
                    fail_name = "STACK_OVERFLOW";
                    break;
                case WRONG_EXPRESSION:
                    fail_name = "WRONG_EXPRESSION";
                    break;
                default:
                    fail_name = "MEMORY_ERROR";
            }
            printf("%s\n", fail_name);
            if (i == argc - 1 || x == STACK_OVERFLOW) {
                StackFree(stack);
                StackFree(res);
                return 0;
            }
            StackClean(stack);
            StackClean(res);
            printf("\n");
            continue;
        }
        printf("all-symbol stack: ");
        StackOutput(stack);
        printf("logical-symbol stack: ");
        StackFakeOutput(res);
        x = Task(res, &result);
        if (x != OK) {
            fail_name = ((x == WRONG_EXPRESSION) ? "WRONG_EXPRESSION" : "MEMORY_ERROR");
            printf("%s\n\n", fail_name);
            if (i == argc - 1) {
                StackFree(stack);
                StackFree(res);
                return 0;
            }
            StackClean(stack);
            StackClean(res);
        } else {
            printf("result: %c\n\n", result);
        }
    }
    StackFree(stack);
    StackFree(res);
    return 0;
}
       

