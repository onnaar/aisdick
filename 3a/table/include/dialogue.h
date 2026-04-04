#ifndef DIALODUE_H
#define DIALODUE_H

#include "table.h"

typedef status (*FunctionArray)(Table *const);

status DoInsert(Table *const table); 
status DoFindVersion(Table *const table); 
status DoFindKey(Table *const table); 
status DoImport(Table *const table); 
status DoDeleteVersion(Table *const table); 
status DoDeleteKey(Table *const table); 
status DoOutput(Table *const table); 
status ProgramEnd(Table *const table); 

#endif
