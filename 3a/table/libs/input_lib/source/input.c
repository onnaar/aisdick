#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "input.h"

#define DELIMS " \t"

InputStatus GetInt(int *const num, int min, int max) {
    if (!num) {
        return ERR;
    }
    int x = 0;
    char ch = '0', f_ch = '0';
    while (ch != '\n') {
        printf("enter the number: ");
        f_ch = getchar();
        if (f_ch == EOF) {
            printf("finished\n");
            return INPUT_EOF;
        } else {
            if (f_ch == '\n') {
                continue;
            } else {
                if (strchr(DELIMS, (int)f_ch)) {
                    scanf("%*["DELIMS"]");
                    f_ch = getchar();
                    if (f_ch == '\n') {
                        continue;
                    }
                }
            }
        }
        if (ungetc((int)f_ch, stdin) == EOF) {
            return ERR;
        }
        x = scanf("%d%c", num, &ch);
        if (x == EOF) {
            printf("finished\n");
            return INPUT_EOF;
        }
        if (x != 2 || ch != '\n') {
            scanf("%*[^\n]%*c");
            printf("invalid input!!\n");
        } else {
            if  (*num < min || *num > max) {
                ch = '0';
                printf("invalid input!!\n");
            }
        }
    }
    return INPUT_OK;
}

InputStatus GetSizeT(size_t *const num) {
    if (!num) {
        return ERR;
    }
    int x = 0;
    long long fake_num = 0;
    char ch = '0', f_ch = '0';
    while (ch != '\n') {
        printf("enter the number: ");
        f_ch = getchar();
  //      printf("\n\nhere %d\n\n", f_ch);
        if (f_ch == EOF) {
            printf("finished\n");
            return INPUT_EOF;
        } else {
            if (f_ch == '\n') {
                continue;
            } else {
                if (strchr(DELIMS, (int)f_ch)) {
                    scanf("%*["DELIMS"]");
                    f_ch = getchar();
                    if (f_ch == '\n') {
                        continue;
                    }
                }
            }
        }
        if (ungetc((int)f_ch, stdin) == EOF) {
            return ERR;
        }
        x = scanf("%lld%c", &fake_num, &ch);
        if (x == EOF) {
            printf("finished\n");
            return INPUT_EOF;
        }
        if (x != 2 || ch != '\n') {
            scanf("%*[^\n]%*c");
            printf("invalid input!!\n");
        } else {
            if  (fake_num < 0) {
                ch = '0';
                printf("invalid input!!\n");
            }
        }
    }
    *num = (size_t)fake_num;
    return INPUT_OK;
}

InputStatus GetString(char **const str) {
    if (!str) {
        return ERR;
    }
    int x = 0;
    char ch = '0', f_ch = '0';
    while (ch != '\n') {
        printf("enter the line: ");
        f_ch = getchar();
        if (f_ch == EOF) {
            printf("finished\n");
            return INPUT_EOF;
        } else {
            if (f_ch == '\n') {
                continue;
            } else {
                if (strchr(DELIMS, (int)f_ch)) {
                    scanf("%*["DELIMS"]");
                    f_ch = getchar();
                    if (f_ch == '\n') {
                        continue;
                    }
                }
            }
        }
        if (ungetc((int)f_ch, stdin) == EOF) {
            return ERR;
        }
        x = scanf("%m[^\n]%c", str, &ch);
        if (x == EOF) {
            printf("finished\n");
            return INPUT_EOF;
        }
    }
    return INPUT_OK;
}
