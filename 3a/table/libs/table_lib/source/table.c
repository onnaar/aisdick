#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "keySpace.h"

Table *TableCreate() {
    return (Table *)calloc(1, sizeof(Table));
}


status TableInsert(Table *const table, KeyType key, const InfoType *const info) {
    if (!table || !info) {
        return NOT_EXIST;
    }
    KeySpace *cur_key_space = table->key_space;
    while(cur_key_space) {
        if (key == cur_key_space->key) {
            break;
        }
        cur_key_space = cur_key_space->link;
    }
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
        cur_key_space->link = table->key_space;
        table->key_space = cur_key_space;
    }
    return OK;
}

status TableDeleteVersion(Table *const table, KeyType key, ReleaseType release) {
    if (!table) {
        return NOT_EXIST;
    }
    KeySpace *cur_key_space = table->key_space, *prev_key_space = NULL;
    while (cur_key_space && cur_key_space->key != key) {
        prev_key_space = cur_key_space;
        cur_key_space = cur_key_space->link;
    }
    if (!cur_key_space) {
        return NOT_FOUND;
    }
    Node *prev_node = NULL, *cur_node = cur_key_space->node;
    while (cur_node && cur_node->release != release) {
        prev_node = cur_node;
        cur_node = cur_node->next;
    }
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
            table->key_space = cur_key_space->link;
        } else {
            prev_key_space->link = cur_key_space->link;
        }
        //KeySpaceDelete(cur_key_space);
        free(cur_key_space);
    }
    return OK;
}

status TableDeleteKey(Table *const table, KeyType key) {
    if (!table) {
        return NOT_EXIST;
    }
    KeySpace *cur_key_space = table->key_space, *prev_key_space = NULL;
    while (cur_key_space && cur_key_space->key != key) {
        prev_key_space = cur_key_space;
        cur_key_space = cur_key_space->link;
    }
    if (!cur_key_space) {
        return NOT_FOUND;
    }
    if (!prev_key_space) {
        table->key_space = cur_key_space->link;
    } else {
        prev_key_space->link = cur_key_space->link;
    }
    KeySpaceDelete(cur_key_space);
    return OK;
}


Table *TableFindVersion(const Table *const table, KeyType key, ReleaseType release) {
    Table *found_table = TableCreate();
    if (!found_table) {
        return NULL;
    }
    KeySpace *cur_key_space = table->key_space;
    while (cur_key_space && cur_key_space->key != key) {
        cur_key_space = cur_key_space->link;
    }
    if (!cur_key_space) {
        TableDelete(found_table);
        return NULL;
    }
    Node *cur_node = cur_key_space->node;
    while (cur_node->release != release) {
        if (!cur_node->next) {
            TableDelete(found_table);
            return NULL;
        }
        cur_node = cur_node->next;

    }
    char *new_info = strdup(cur_node->info);
    TableInsert(found_table, key, new_info);
    free(new_info);
    return found_table;
} 

Table *TableFindKey(const Table *const table, KeyType key) {
    Table *found_table = TableCreate();
    if (!found_table) {
        return NULL;
    }
    KeySpace *cur_key_space = table->key_space;
    while (cur_key_space && cur_key_space->key != key) {
        cur_key_space = cur_key_space->link;
    }
    if (!cur_key_space) {
        TableDelete(found_table);
        return NULL;
    }
    Node *cur_node = cur_key_space->node;
    while (cur_node) {
        char *new_info = strdup(cur_node->info);
        TableInsert(found_table, key, new_info);
        free(new_info);
        cur_node = cur_node->next;
    }
    return found_table;
} 

status TableImport(Table *const table, const char *const filename) {
    if (!table) {
        return NOT_EXIST;
    }
    FILE *file = fopen(filename, "r");
    if (!file) {
        return NOT_FOUND;
    }
    KeyType key = 0;
    InfoType *info = NULL;
    while (fscanf(file, "%zu %ms", &key, &info) == 2) {
        TableInsert(table, key, info);
        free(info);
    }
    fclose(file);
    return OK;
}

status TableOutput(const Table *const table) {
    if (!table || !table->key_space || !table->key_space->node) {
        return TABLE_EMPTY;
    }
    printf("\n%-10s | %-8s | %-20s\n", "Key", "Rel", "Info");
    printf("-----------|----------|----------------------\n");
    KeySpace *cur_key_space = table->key_space;
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

void TableDelete(Table *table) {
    if (!table) {
        return;
    }
    KeySpace *cur = table->key_space;
    while (cur) {
        KeySpace *temp = cur;
        cur = cur->link;
        KeySpaceDelete(temp);
    } 
    free(table);
}
