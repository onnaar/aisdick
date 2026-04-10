#ifndef DIALODUE_H
#define DIALODUE_H

#include "table.h"

typedef TableStatus (*FunctionArray)(Table *const);

TableStatus DoInsert(Table *const table); 
TableStatus DoFindVersion(Table *const table); 
TableStatus DoFindKey(Table *const table); 
TableStatus DoImport(Table *const table); 
TableStatus DoExport(Table *const table);
TableStatus DoDeleteVersion(Table *const table); 
TableStatus DoDeleteKey(Table *const table); 
TableStatus DoOutput(Table *const table); 
TableStatus ProgramEnd(Table *const table); 

#endif
