#ifndef INPUT_H
#define INPUT_H 

#include <stdio.h>

typedef enum {
    INPUT_EOF = -1,
    ERR,
    INPUT_OK
} InputStatus;

InputStatus GetInt(int *const, int, int);
InputStatus GetSizeT(size_t *const);
InputStatus GetString(char **const);

#endif
