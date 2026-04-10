#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "input.h"
#include "table.h"

#define PROMT "> "
#define DELIM " \n\t"
#define MAGIC_WORD "TABLE_STRUCTURE"
#define FORMAT_CHECK(contidion, buffer) \
        if (contidion) {   \
            free(buffer);  \
            continue;   \
        }

Table *TableCreate() {
    return (Table *)calloc(1, sizeof(Table));
}

KeySpace *FindKey(Table *table, KeyType key, KeySpace **cur) {
    if (!cur || !table) {
        return NULL;
    }
    *cur = table->head;
    KeySpace *prev = NULL;
    while (*cur) {
        if (key == (*cur)->key) {
            break;
        }
        prev = *cur;
        *cur = (*cur)->link;
    }
    return prev;
}

Node *FindRelease(KeySpace *cur_key_space, ReleaseType release, Node **cur) {
    if (!cur || !cur_key_space) {
        return NULL;
    }
    *cur = cur_key_space->node;
    Node *prev = NULL;
    while (*cur) {
        if (release == (*cur)->release) {
            break;
        }
        prev = *cur;
        *cur = (*cur)->next;
    }
    return prev;
}

TableStatus TableInsert(Table *const table, KeyType key, const InfoType *const info) {
    if (!table || !info) {
        return NOT_EXIST;
    }
    KeySpace *cur_key_space = NULL;
    FindKey(table, key, &cur_key_space);
    if (cur_key_space) {
        Node *cur = NodeCreate(info, cur_key_space->node->release);
        if (!cur) {
            return MEMORY_ERROR;
        }
        cur->next = cur_key_space->node;
        cur_key_space->node = cur;
    } else {
        cur_key_space = KeySpaceCreate(info, key);
        if (!cur_key_space) {
            return MEMORY_ERROR;
        }
        cur_key_space->link = table->head;
        table->head = cur_key_space;
    }
    return OK;
}

TableStatus TableInsertRelease(Table *const table, KeyType key, const InfoType *const info, const ReleaseType release) {
    if (!table || !info) {
        return NOT_EXIST;
    }
    KeySpace *cur_key_space = NULL;
    FindKey(table, key, &cur_key_space);
    if (cur_key_space) {
        Node *is_existing = NULL;
        FindRelease(cur_key_space, release, &is_existing);
        if (is_existing) {
            return RELEASE_DUPLICATE;
        }
        Node *cur = NodeCreateRelease(info, release);
        if (!cur) {
            return MEMORY_ERROR;
        }
        cur->next = cur_key_space->node;
        cur_key_space->node = cur;
    } else {
        cur_key_space = KeySpaceCreateRelease(info, key, release);
        if (!cur_key_space) {
            return MEMORY_ERROR;
        }
        cur_key_space->link = table->head;
        table->head = cur_key_space;
    }
    return OK;
}

TableStatus TableDeleteVersion(Table *const table, KeyType key, ReleaseType release) {
    if (!table) {
        return NOT_EXIST;
    }
    KeySpace *cur_key_space = NULL;
    KeySpace *prev_key_space = FindKey(table, key, &cur_key_space);
    if (!cur_key_space) {
        return NOT_FOUND;
    }
    Node *cur_node = NULL;
    Node *prev_node = FindRelease(cur_key_space, release, &cur_node);
    if (!cur_node) {
        return NOT_FOUND;
    }
    if (!prev_node) {
        cur_key_space->node = cur_node->next;
    } else {
        prev_node->next = cur_node->next;
    }
    NodeDelete(cur_node);
    if (!cur_key_space->node) {
        if (!prev_key_space) {
            table->head = cur_key_space->link;
        } else {
            prev_key_space->link = cur_key_space->link;
        }
        //KeySpaceDelete(cur_key_space);
        free(cur_key_space);
    }

    return OK;
}

TableStatus TableDeleteKey(Table *const table, KeyType key) {
    if (!table) {
        return NOT_EXIST;
    }
    KeySpace *cur_key_space = NULL;
    KeySpace *prev_key_space = FindKey(table, key, &cur_key_space);
    if (!cur_key_space) {
        return NOT_FOUND;
    }
    if (!prev_key_space) {
        table->head = cur_key_space->link;
    } else {
        prev_key_space->link = cur_key_space->link;
    }
    KeySpaceDelete(cur_key_space);
    return OK;
}

Table *TableFindVersion(Table *const table, KeyType key, ReleaseType release) {
    Table *found_table = TableCreate();
    if (!found_table) {
        return NULL;
    }
    KeySpace *cur_key_space = NULL;
    FindKey(table, key, &cur_key_space);
    if (!cur_key_space) {
        TableDelete(found_table);
        return NULL;
    }
    Node *cur_node = NULL;
    FindRelease(cur_key_space, release, &cur_node);
    if (!cur_node) {
        TableDelete(found_table);
        return NULL;
    }
    char *new_info = strdup(cur_node->info);
    if (!new_info) {
        TableDelete(table);
        return NULL;
    }
    TableInsertRelease(found_table, key, new_info, cur_node->release);
    free(new_info);
    return found_table;
} 

Table *TableFindKey(Table *const table, KeyType key) {
    Table *found_table = TableCreate();
    if (!found_table) {
        return NULL;
    }
    KeySpace *cur_key_space = NULL;
    FindKey(table, key, &cur_key_space);
    if (!cur_key_space) {
        TableDelete(found_table);
        return NULL;
    }
    Node *cur_node = cur_key_space->node;
    while (cur_node) {
        char *new_info = strdup(cur_node->info);
        if (!new_info) {
            TableDelete(table);
            return NULL;
        }
        TableInsertRelease(found_table, key, new_info, cur_node->release);
        free(new_info);
        cur_node = cur_node->next;
    }
    return found_table;
} 

TableStatus TableImport(Table *const table, const char *const filename) {
    if (!table) {
        return NOT_EXIST;
    }
    FILE *file = fopen(filename, "r");
    if (!file) {
        return NOT_FOUND;
    }
    char *magic = my_readline(file, "");
    if (!magic || (strcmp(magic, MAGIC_WORD)) != 0) {
        if (magic) {
            free(magic);
        }
        fclose(file);
        return WRONG_FORMAT;
    }
    free(magic);
    char *buffer = NULL;
    InfoType *info = NULL;
    KeyType key = 0;
    ReleaseType release = 0;
    while ((buffer = my_readline(file, PROMT)) != NULL) {
        char *word = strtok(buffer, DELIM);
        FORMAT_CHECK(word == NULL, buffer)
        StrToZu(word, &key);
        word = strtok(NULL, DELIM);
        FORMAT_CHECK(word == NULL, buffer)
        StrToZu(word, &release);
        FORMAT_CHECK(release == 0, buffer)
        word = strtok(NULL, "");
        FORMAT_CHECK(word == NULL, buffer)
        info = strdup(word);
        if (info) {
            TableStatus stat = TableInsertRelease(table, key, info, release);
            if (stat == RELEASE_DUPLICATE) {
                printf("\ncurrent node with key = %zu and release = %zu - duplicate, hasnt been inserted\n", key, release);
            }
            free(info);
        }
        free(buffer);
    }
    fclose(file);
    return OK;
}

TableStatus TableExport(const Table *const table, const char *const filename) {
    if (!table || !filename) {
        return NOT_EXIST;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        return NOT_FOUND;
    }
    fprintf(file, "%s\n", MAGIC_WORD);
    KeySpace *cur_key_space = table->head;
    Node *cur_node = NULL;
    while (cur_key_space) {
        cur_node = cur_key_space->node;
        while (cur_node) {
            fprintf(file, "%zu %zu %s\n", cur_key_space->key, cur_node->release, cur_node->info);
            cur_node = cur_node->next;
        }
        cur_key_space = cur_key_space->link;
    }
    fclose(file);
    return OK;
}

TableStatus TableOutput(const Table *const table) {
    if (!table || !table->head || !table->head->node) {
        return TABLE_EMPTY;
    }
    printf("\n%-10s | %-8s | %-20s\n", "Key", "Rel", "Info");
    printf("-----------|----------|----------------------\n");
    KeySpace *cur_key_space = table->head;
    while (cur_key_space) {
        printf("%-10zu | %-8zu | %-20s\n", cur_key_space->key, cur_key_space->node->release, cur_key_space->node->info);
        Node *cur_node = cur_key_space->node->next;
        while (cur_node) {
            printf("%-10s | %-8zu | %-20s\n", "", cur_node->release, cur_node->info);
            cur_node = cur_node->next;
        }
        printf("-----------|----------|----------------------\n");
        cur_key_space = cur_key_space->link;
    }
    return OK;
}

TableStatus TableDownOutput(const Table *const table) {
    if (!table || !table->head || !table->head->node) {
        return TABLE_EMPTY;
    }
    printf("\n%-10s | %-8s | %-20s\n", "Key", "Rel", "Info");
    printf("-----------|----------|----------------------\n");
    KeySpace *cur_key_space = table->head;
    while (cur_key_space) {
        KeySpaceDownOutput(cur_key_space);
        printf("-----------|----------|----------------------\n");
        cur_key_space = cur_key_space->link;
    }
    return OK;
}

void TableDelete(Table *table) {
    if (!table) {
        return;
    }
    KeySpace *cur = table->head;
    while (cur) {
        KeySpace *temp = cur;
        cur = cur->link;
        KeySpaceDelete(temp);
    } 
    free(table);
}
