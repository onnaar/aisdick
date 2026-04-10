#ifndef TABLE_H
#define TABLE_H

#include "keySpace.h"
#include "node.h"
// массив указателей которые будут вызывать диалоговые функции. в одном каталоге с меин 
typedef enum {
    OK = 0,
    NOT_EXIST,
    NOT_FOUND,
    TABLE_EMPTY,
    MEMORY_ERROR,
    RELEASE_DUPLICATE,
    WRONG_FORMAT,
    END_OF_INPUT
} TableStatus;

typedef struct {
    struct KeySpace *head;
} Table;

Table *TableCreate();

KeySpace *FindKey(Table *talbe, KeyType key, KeySpace **cur);
Node *FindRelease(KeySpace *cur_key_space, ReleaseType release, Node **cur);
TableStatus TableInsert(Table *const table, const KeyType key, const InfoType *const info);
TableStatus TableInsertRelease(Table *const table, const KeyType key, const InfoType *const info, const ReleaseType release);
TableStatus TableImport(Table *const table, const char *const filename);
TableStatus TableExport(const Table *const table, const char *const filename);
Table *TableFindVersion(Table *const table, KeyType key, ReleaseType release);
Table *TableFindKey(Table *const table, KeyType key);

TableStatus TableOutput(const Table *const table);
TableStatus TableDownOutput(const Table *const table);

TableStatus TableDeleteVersion(Table *const table, KeyType key, ReleaseType release);
TableStatus TableDeleteKey(Table *const table, KeyType key);
void TableDelete(Table *table);

#endif
