#ifndef INPUT_H
#define INPUT_H 

#include <stddef.h>
#include <stdio.h>

typedef enum {
    INPUT_EOF = -1,
    ERR,
    INPUT_OK,
    INPUT_WRONG
} InputStatus;

InputStatus GetInt(int *const, int, int);
InputStatus GetSizeT(size_t *const);
InputStatus GetString(char **const);
char *my_strtok(char *, const char *);
char *my_readline(FILE *, const char *); 
InputStatus StrToZu(const char *const str, size_t *num);


#endif
