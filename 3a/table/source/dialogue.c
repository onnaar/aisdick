#include <stdio.h>
#include <stdlib.h>
#include "dialogue.h"
#include "input.h"
#include "table.h"

status DoInsert(Table *const table) {
    if (!table) {
        return NOT_EXIST;
    } 
    printf("enter the key:\n");
    KeyType key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
       return END_OF_INPUT;
    }
    printf("enter the information:\n");
    InfoType *info = NULL;
    stat = GetString(&info);
    if (stat != INPUT_OK) {
       free(info);
       return END_OF_INPUT;
    } 
    status proc_stat = TableInsert(table, key, info);
    free(info);
    if (proc_stat != OK) {
        return proc_stat;
    }
    return OK;
}

status DoImport(Table *const table) {
    if (!table) {
        return NOT_EXIST;
    }
    char *filename = NULL;
    printf("enter name of the file:\n");
    InputStatus stat = GetString(&filename);
    if (stat != INPUT_OK) {
        free(filename);
        return END_OF_INPUT;
    }
    status proc_stat = TableImport(table, filename);
    free(filename);
    if (proc_stat != OK) {
        return proc_stat;
    }
    return OK;
}

status DoFindVersion(Table *const table) {
    if (!table) {
        return NOT_EXIST;
    }
    printf("enter the search key:\n");
    KeyType key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
       return END_OF_INPUT;
    }
    ReleaseType release = 0;
    printf("enter the search release:\n");
    stat = GetSizeT(&release);
    if (stat != INPUT_OK) {
        return END_OF_INPUT;
    }
    Table *res_table = TableFindVersion(table, key, release);
    if (!res_table) {
        return NOT_FOUND;
    }
    TableOutput(res_table);
    TableDelete(res_table);
    return OK;
}

status DoFindKey(Table *const table) {
    if (!table) {
        return NOT_EXIST;
    }
    printf("enter the search key:\n");
    KeyType key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
       return END_OF_INPUT;
    }
    Table *res_table = TableFindKey(table, key);
    if (!res_table) {
        return NOT_FOUND;
    }
    TableOutput(res_table);
    TableDelete(res_table);
    return OK;
}

status DoOutput(Table *const table) {
    if (!table) {
        return NOT_EXIST;
    }
    status stat = TableOutput(table);
    if (stat != OK) {
        return stat;
    }
    return OK;
}

status DoDeleteVersion(Table *const table) {
    if (!table) {
        return NOT_EXIST;
    }
    printf("enter deleting key:\n");
    KeyType key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
       return END_OF_INPUT;
    }
    ReleaseType release = 0;
    printf("enter deleting release:\n");
    stat = GetSizeT(&release);
    if (stat != INPUT_OK) {
        return END_OF_INPUT;
    }
    status proc_stat = TableDeleteVersion(table, key, release);
    if (proc_stat != OK) {
        return proc_stat;
    }
    return OK;
}

status DoDeleteKey(Table *const table) {
    if (!table) {
        return NOT_EXIST;
    }
    printf("enter deleting key:\n");
    KeyType key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
       return END_OF_INPUT;
    }
    status proc_stat = TableDeleteKey(table, key);
    if (proc_stat != OK) {
        return proc_stat;
    }
    return OK;
}

status ProgramEnd(Table *const table) {
    (void )table;
    return OK;
}
