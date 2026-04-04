#ifndef TABLE_H
#define TABLE_H

#include "keySpace.h"
// массив указателей которые будут вызывать диалоговые функции. в одном каталоге с меин 
typedef enum {
    OK = 0,
    NOT_EXIST,
    NOT_FOUND,
    TABLE_EMPTY,
    MEMORY_ERROR,
    END_OF_INPUT
} status;

typedef struct {
    struct KeySpace *key_space;
} Table;

Table *TableCreate();

status TableInsert(Table *const table, const KeyType key, const InfoType *const info);
status TableImport(Table *const table, const char *const filename);
Table *TableFindVersion(const Table *const table, KeyType key, ReleaseType release);
Table *TableFindKey(const Table *const table, KeyType key);

status TableOutput(const Table *const table);

status TableDeleteVersion(Table *const table, KeyType key, ReleaseType release);
status TableDeleteKey(Table *const table, KeyType key);
void TableDelete(Table *table);

#endif
