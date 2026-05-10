#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "node.h"
#include "tree.h"

size_t *RandomGenerator(const size_t size);
size_t *ManualGenerator(const size_t size);
void WriteNumBin(const char *const filename, const size_t *const numbers, const size_t count);

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("NO PARAMETERS!!\n");
        return 0;
    }
    char *filename = NULL;
    size_t cur_pos = 0;
    for (int i = 1; i < argc; i++) {
        size_t word_len = strlen(argv[i]);
        char *new_filename = (char *)realloc(filename, sizeof(char) * (cur_pos + word_len + 2));
        if (!new_filename) {
            goto exit;
        }
        filename = new_filename;
        strcpy(filename + cur_pos, argv[i]);
        cur_pos += word_len + 1;
        filename[cur_pos - 1] = (i + 1 != argc) ? ' ' : '\0';
    }
    printf("filename: %s\n", filename);
    size_t size = 0;
    int choice = 0;
    printf("1 - manual, 2 - random, 3 - from existing\n");
    InputStatus stat = GetInt(&choice, 1, 3);
    if (stat != INPUT_OK) {
        goto exit;
    }
    if (choice != 3) {
        printf("size:\n");
        stat = GetSizeT(&size);
        if (stat != INPUT_OK) {
            goto exit;
        }
        size_t *array = (choice == 1) ? ManualGenerator(size) : RandomGenerator(size);
        if (!array) {
            goto exit;
        }
        WriteNumBin(filename, array, size);
        free(array);
    }
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("sry error during openning\n");
        goto exit;
    }
    free(filename);
    size_t key = 0;
    Tree *tree = TreeCreate();
    TreeStatus status = TREE_OK;
    while (fread(&key, sizeof(size_t), 1, file)) {
        Node *cur_node = FindKeyRelease(tree, key, 1);
        if (!cur_node) {
            NodeInfo *info = NodeInfoCreate();
            if (!info) {
                goto exit;
            }
            info->info = 1;
            status = TreeInsert(tree, key, info);
            if (status != TREE_OK) {
                goto exit;
            }
            free(info);
        } else {
            cur_node->info->info++;
        }
    }
    printf("where should write data\n");
    stat = GetString(&filename);
    if (stat != INPUT_OK) {
        goto exit;
    }
    status = TreeExport(tree, filename);
    if (status != TREE_OK) {
        printf("\nsad\n");
        goto exit;
    }
    TreeTraversing(tree, Delete, NULL);
    free(tree);
    fclose(file);
exit:
    free(filename);
    return 0;
}

size_t *RandomGenerator(const size_t size) {
    size_t *numbers = (size_t *)calloc(size, sizeof(size_t));
    if (!numbers) {
        return NULL;
    }
    for (size_t i = 0; i < size; i++) {
        *(numbers + i) = (((size_t)rand() << 32) | (size_t)rand()) % 5;
    }
    for (size_t i = 0; i < size; i++) {
        printf("%zu ", *(numbers + i));
    }
    printf("\n\n\n");
    return numbers;
}

size_t *ManualGenerator(const size_t size) {
    size_t *numbers = (size_t *)calloc(size, sizeof(size_t));
    if (!numbers) {
        return NULL;
    }
    for (size_t i = 0; i < size; i++) {
        size_t cur_value = 0;
        InputStatus stat = GetSizeT(&cur_value);
        if (stat != INPUT_OK) {
            free(numbers);
            return NULL;
        }
        *(numbers + i) = cur_value;
    }
    return numbers;
}

void WriteNumBin(const char *const filename, const size_t *const numbers, const size_t count) {
    if (!numbers || !filename) {
        return;
    }
    FILE *file = fopen(filename, "wb");
    if (!file) {
        return;
    }
    size_t written = fwrite(numbers, sizeof(size_t), count, file);
    if (written != count) {
        printf("suck");
    }
    fclose(file);
    return;
}
