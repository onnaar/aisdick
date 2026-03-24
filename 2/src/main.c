#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "process.h"
#include "stack.h"
#ifdef STACK_VECTOR
    #include "input.h"
#endif

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("NO PARAMETERS!!\n");
        return 0;
    }
    char *fail_name[] = {"MEMORY_ERROR", "NULLPTR", "OK", "STACK_OVERFLOW", "STACK_EMPTY", "WRONG_EXPRESSION"};
    size_t size = 0;
    Stack *input_stack = NULL;
    Stack *logic_only_stack = NULL;
    status x = OK;
    for (int i = 1; i < argc; i++) {
#ifdef STACK_VECTOR
        printf("stack's size:\n");
        InputStatus y = GetSizeT(&size);
        if (y != INPUT_OK) {
            printf("INPUT_EOF\n");
            if (input_stack) {
                StackFree(input_stack);
            }
            if (logic_only_stack) {
                StackFree(logic_only_stack);
            }
            return 0;
        }
#endif
        x = Processing(argv[i], &input_stack, &logic_only_stack, size);
        if (x != OK) {
            printf("%s\n", fail_name[x]);
        }
        StackFree(input_stack);
        input_stack = NULL;
        StackFree(logic_only_stack);
        logic_only_stack = NULL;
    }
    if (input_stack) {
        StackFree(input_stack);
    }
    if (logic_only_stack) {
        StackFree(logic_only_stack);
    }
    return 0;
}
