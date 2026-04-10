#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

InputStatus StrToZu(const char *const str, size_t *num) {
    if (!str || !num) {
        return INPUT_WRONG;
    }
    size_t fake_num = 0, i = 0;
    char *new_str = strdup(str);
    if (!new_str) {
        return ERR;
    }
    bool is_ok = true;
    while (*(new_str + i) && is_ok) {
        if (*(new_str + i) >= '0' && *(new_str + i) <= '9') {
            fake_num = 10 * fake_num + (*(new_str + i) - '0');
        } else {
            is_ok = false;
        }
        i++;
    }
    *num = fake_num;
    free(new_str);
    return (is_ok ? INPUT_OK : INPUT_WRONG);
}

char *my_strtok(char *str, const char *delim) {
    static char *cur_ptr = NULL;
    char *token = NULL;
    if (str == NULL) {
        str = cur_ptr;
        if (str == NULL) {
            return NULL;
        }
    }
    str += strspn(str, delim);
    if (*str == '\0') {
        cur_ptr = NULL;
        return NULL;
    }
    token = str;
    str = strpbrk(token, delim);
    if (str != NULL) {
        *str = '\0';
        cur_ptr = str + 1;
    } else {
        cur_ptr = NULL;
    }
    return token;
}

char *my_readline(FILE *file, const char *PROMT) {
    char buf[100] = {0}; 
    char *res = NULL;
    int len = 0;
    int n = 0;
    if (PROMT && file == stdin) {
        printf("%s", PROMT);
    }
    do {
        n = fscanf(file, "%99[^\n]", buf);
        if (n < 0 && !res) {
            return NULL;
        } else if (n > 0) {
            size_t chunk_len = 0;
            chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            char *new_res = (char *)realloc(res, str_len + 1);
            if (!new_res) {
                free(res);
                return NULL;
            }
            res = new_res;
            memcpy(res + len, buf, chunk_len);
            len = str_len;
            } else {
                fgetc(file);
            }
    } while (n > 0);
    if (len > 0) {
        res[len] = '\0';
    } else {
        res = (char *)calloc(1, sizeof(char));
    }
    return res;
}
