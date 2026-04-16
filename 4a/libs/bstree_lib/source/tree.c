#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tree.h"
#include "node.h"
#include "stack.h"

Tree *TreeCreate(size_t key, size_t *info) {
    if (!info) {
        return NULL;
    }
    Tree *tree = (Tree *)calloc(1, sizeof(Tree));
    if (!tree) {
        return NULL;
    }
    tree->root = NodeCreate(NULL, key, info);
    if (!tree->root) {
        return NULL;
    }
    return tree;
}

Node *FindKey(Tree *tree, size_t key) {
    if (!tree) {
        return NULL;
    }
    Node *cur = tree->root;
    RelativeIndex index = LEFT;
    while (cur && cur->key != key) {
        index = LEFT;
        if (key >= cur->key) {
            index = RIGHT;
        }
        cur = cur->relatives[index];
    }
    return cur;
}

Node *FindMinKey(Node *node) {
    if (!node) {
        return NULL;
    }
    Node *cur = node, *prev = NULL;
    while (cur) {
        prev = cur;
        cur = cur->relatives[LEFT]; 
    }
    return prev;
}

Node *FindNextKey(Tree *tree, size_t key) {
    if (!tree) {
        return NULL;
    }
    Node *cur = tree->root;
    RelativeIndex index = LEFT;
    while (cur && cur->key != key) {
        index = LEFT;
        if (key >= cur->key) {
            index = RIGHT;
        }
        cur = cur->relatives[index];
    }
    if (!cur) {
        return NULL;
    }
    Node *node_possible = FindMinKey(cur->relatives[RIGHT]);
   /* if (node_possible) {
        return node_possible;
    }*/
    if (!node_possible) {
        Node *parent = cur->relatives[PARENT];
        while (parent && cur == parent->relatives[RIGHT]) {
            cur = parent;
            parent = parent->relatives[PARENT];
        }
        return parent;
    }
    return node_possible;
}

TreeStatus TreeInsert(Tree *tree, size_t key, size_t *info) {
    if (!tree || !info) {
        return NOT_EXIST;
    }
    Node *cur = tree->root, *prev = NULL;
    RelativeIndex index = LEFT;
    while (cur) {
        prev = cur;
        if (key > cur->key) {
            index = RIGHT;
        } else if (key < cur->key) {
            index = LEFT;
        } else { 
            while (cur && key == cur->key) {
                prev = cur;
                cur = cur->relatives[RIGHT];
            }
            break;
        }
        cur = cur->relatives[index];
    }
    Node *temp = NodeCreate(prev, key, info);
    if (!temp) {
        return MEMORY_ERROR;
    }
    if (!tree->root) {
       tree->root = temp;
       return OK;
    }
    if (cur) {
        prev->relatives[RIGHT] = temp;
        temp->relatives[RIGHT] = cur;
        temp->relatives[PARENT] = prev;
        cur->relatives[PARENT] = temp;
    } else {
        prev->relatives[index] = temp;
    }
    return OK;
}

TreeStatus TreeKeyDelete(Tree *tree, size_t key) {
    if (!tree) {
        return NOT_EXIST;
    }
    Node *target = NULL;
    target = FindKey(tree, key);
    if (!target) {
        return NOT_FOUND;
    }
    Node *parent = target->relatives[PARENT];
    if (!target->relatives[LEFT] && !target->relatives[RIGHT]) {
        parent->relatives[key < parent->key ? LEFT : RIGHT] = NULL;
        NodeDelete(target);
        return OK;
    }
    RelativeIndex index = LEFT; 
    if (!target->relatives[LEFT] || !target->relatives[RIGHT]) {
        if (!target->relatives[LEFT]) {
            index = RIGHT;
        }
        target->relatives[index]->relatives[PARENT] = target->relatives[PARENT];
        NodeDelete(target);
        return OK;
    }
    Node *successor = FindNextKey(tree, key);
    target->key = successor->key;
    target->info = successor->info;
    if (successor->relatives[PARENT]->relatives[LEFT] == successor) {
        successor->relatives[PARENT]->relatives[LEFT] = successor->relatives[RIGHT];
        if (successor->relatives[RIGHT]) {
            successor->relatives[RIGHT]->relatives[PARENT] = successor->relatives[PARENT];
        }
    } else {
        successor->relatives[PARENT]->relatives[RIGHT] = successor->relatives[RIGHT];
        if (successor->relatives[RIGHT]) {
            successor->relatives[RIGHT]->relatives[PARENT] = successor->relatives[PARENT];
        }
    }
    NodeDelete(successor);
    return OK;
}

TreeStatus TreeOutput(Tree *tree) {
    if (!tree) {
        return NOT_EXIST;
    }
    if (!tree->root) {
        return TREE_EMPTY;
    }
    Stack *stack = StackCreate();
    
}
