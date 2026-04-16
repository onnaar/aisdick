#include <stdio.h>
#include <stdlib.h>
#include "dialogue.h"
#include "input.h"
#include "table.h"

TableStatus DoInsert(Table *const table) {
    if (!table) {
        return NOT_VALID;
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
    TableStatus proc_stat = TableInsert(table, key, info);
    free(info);
    if (proc_stat != OK) {
        return proc_stat;
    }
    return OK;
}

TableStatus DoImport(Table *const table) {
    if (!table) {
        return NOT_VALID;
    }
    char *filename = NULL;
    printf("enter name of the file regarding current directory:\n");
    InputStatus stat = GetString(&filename);
    if (stat != INPUT_OK) {
        free(filename);
        return END_OF_INPUT;
    }
    TableStatus proc_stat = TableImport(table, filename);
    free(filename);
    if (proc_stat != OK) {
        return proc_stat;
    }
    return OK;
}

TableStatus DoExport(Table *const table) {
    if (!table) {
        return NOT_VALID;
    }
    char *filename = NULL;
    printf("enter name of the file:\n");
    InputStatus stat = GetString(&filename);
    if (stat != INPUT_OK) {
        free(filename);
        return END_OF_INPUT;
    }
    TableStatus proc_stat = TableExport(table, filename);
    free(filename);
    if (proc_stat != OK) {
        return proc_stat;
    }
    return OK;
}

TableStatus DoFindVersion(Table *const table) {
    if (!table) {
        return NOT_VALID;
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

TableStatus DoFindKey(Table *const table) {
    if (!table) {
        return NOT_VALID;
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
    TableDownOutput(res_table);
    //TableOutput(res_table);
    TableDelete(res_table);
    return OK;
}

TableStatus DoOutput(Table *const table) {
    if (!table) {
        return NOT_VALID;
    }
    TableStatus stat = TableOutput(table);
    if (stat != OK) {
        return stat;
    }
    return OK;
}

TableStatus DoDeleteVersion(Table *const table) {
    if (!table) {
        return NOT_VALID;
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
    TableStatus proc_stat = TableDeleteVersion(table, key, release);
    if (proc_stat != OK) {
        return proc_stat;
    }
    return OK;
}

TableStatus DoDeleteKey(Table *const table) {
    if (!table) {
        return NOT_VALID;
    }
    printf("enter deleting key:\n");
    KeyType key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
       return END_OF_INPUT;
    }
    TableStatus proc_stat = TableDeleteKey(table, key);
    if (proc_stat != OK) {
        return proc_stat;
    }
    return OK;
}

TableStatus ProgramEnd(Table *const table) {
    (void)table;
    return OK;
}
