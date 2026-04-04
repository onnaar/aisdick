#ifndef NODE_H
#define NODE_H

#include <stdio.h>

typedef char InfoType;
typedef size_t ReleaseType;

typedef struct Node {
   struct Node *next;
   InfoType *info;
   ReleaseType release;
} Node;

Node *NodeCreate(const InfoType *const info, ReleaseType last_rel);
void NodeDelete(Node *node);

#endif 
