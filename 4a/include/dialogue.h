#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "tree.h"

typedef TreeStatus (*FunctionArray)(Tree *const);

TreeStatus DoInsert(Tree *const tree); 
TreeStatus DoFindKey(Tree *const tree); 
TreeStatus DoFindRelease(Tree *const tree); 
TreeStatus DoSpecialSearch(Tree *const tree);
TreeStatus DoDeleteKey(Tree *const tree); 
TreeStatus DoOutput(Tree *const tree); 
TreeStatus ProgramEnd(Tree *const tree); 

#endif
