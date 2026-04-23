#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "tree.h"
#include "node.h"
#include "stack.h"

Tree *TreeCreate(void) {
    Tree *tree = (Tree *)calloc(1, sizeof(Tree));
    if (!tree) {
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
        return TREE_NOT_EXIST;
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
            index = RIGHT;
            break;
        }
        cur = cur->relatives[index];
    }
    Node *temp = NodeCreate(prev, key, info);
    if (!temp) {
        return TREE_MEMORY_ERROR;
    }
    if (!tree->root) {
       tree->root = temp;
       return TREE_OK;
    }
    if (cur) {
        prev->relatives[RIGHT] = temp;
        temp->relatives[RIGHT] = cur;
        temp->relatives[PARENT] = prev;
        cur->relatives[PARENT] = temp;
    } else {
        prev->relatives[index] = temp;
    }
    return TREE_OK;
}

TreeStatus TreeKeyDelete(Tree *tree, size_t key) {
    if (!tree) {
        return TREE_NOT_EXIST;
    }
    Node *target = FindKey(tree, key);
    if (!target) {
        return TREE_NOT_FOUND;
    }
    Node *parent = target->relatives[PARENT];
    if (!target->relatives[LEFT] && !target->relatives[RIGHT]) {
        if (!parent) {
            tree->root = NULL;
        } else {
            parent->relatives[key < parent->key ? LEFT : RIGHT] = NULL;
        }
        NodeDelete(target);
        return TREE_OK;
    }
    RelativeIndex index = LEFT; 
    if (!target->relatives[LEFT] || !target->relatives[RIGHT]) {
        if (!target->relatives[LEFT]) {
            index = RIGHT;
        }
        Node *child = target->relatives[index];
        Node *parent = target->relatives[PARENT];
        child->relatives[PARENT] = parent;
        if (!parent) {
            tree->root = child;
        } else {
            parent->relatives[(parent->relatives[LEFT] == target) ? LEFT : RIGHT] = child;
        }
        NodeDelete(target);
        return TREE_OK;
    }
    Node *successor = FindNextKey(tree, key);
    target->key = successor->key;
    target->info = successor->info;
    
    index = ((successor->relatives[PARENT]->relatives[LEFT] == successor) ? LEFT : RIGHT);
    successor->relatives[PARENT]->relatives[index] = successor->relatives[RIGHT];
    if (successor->relatives[RIGHT]) {
        successor->relatives[RIGHT]->relatives[PARENT] = successor->relatives[PARENT];
    }
    NodeDelete(successor);
    return TREE_OK;
}

NodeArray *FindKeyRelease(Tree *tree, size_t key, size_t release) {
    if (!tree) {
        return NULL;
    }
    Node *cur = FindKey(tree, key);
    if (!cur) {
        return NULL;
    }
    if (release != 1) {
        size_t i = 1;
        for (; cur && cur->key == key && i != release; i++) {
            cur = cur->relatives[RIGHT];
        }
        if (i != release) {
            return NULL;
        }
    }
    NodeArray *array = NodeArrayManage(NULL, 1);
    if (!array) {
        return NULL;
    }
    *(array->node_array) = cur;
    if (!*(array->node_array)) {
        return NULL;
    }
    array->size = 1;
    return array;
}

void Output(Node *cur, void *context) {
    (void)context;
    printf("%zu %zu\n", cur->key, *cur->info);
}

void Special(Node *cur, void *context) {
    SpSearchStructure *data = (SpSearchStructure *)context; 
    size_t cur_info = *(cur->info);
    size_t delta = (cur_info > data->info) ? (cur_info - data->info) : (data->info - cur_info);
    if (delta > data->max_delta) {
        data->max_delta = delta;
        data->array->size = 1;
    } else if (delta == data->max_delta) {
        data->array->size++;
    }
}

void NodeArrayAdd(NodeArray *array, Node *node) {
    if (!array || !node || !array->node_array) {
        return;
    }
    array->node_array[array->size] = node;
    array->size++;
}

void AllSpecialNodes(Node *cur, void *context) {
    SpSearchStructure *data = (SpSearchStructure *)context; 
    size_t cur_info = *(cur->info);
    size_t delta = (cur_info > data->info) ? (cur_info - data->info) : (data->info - cur_info);
    if (delta == data->max_delta) {
        NodeArrayAdd(data->array, cur);
    }
}

TreeStatus TreeTraversing(Tree *tree, void (*action)(Node *cur, void *context), void *context) {
    if (!tree) {
        return TREE_NOT_EXIST;
    }
    if (!tree->root) {
        return TREE_EMPTY;
    }
    Stack *stack = StackCreate();
    if (!stack) {
        return TREE_MEMORY_ERROR;
    }
    Node *cur = tree->root;
    while (cur || !IsEmpty(stack)) {
        while (cur) {
            StackPush(stack, cur);
            cur = cur->relatives[RIGHT];
        }
        cur = StackPop(stack);
        action(cur, context);
        cur = cur->relatives[LEFT];
    }
    StackFree(stack);
    return TREE_OK;    
}

SpSearchStructure *SpecialSearch(Tree *tree, size_t info) {
    if (!tree || !tree->root) {
        return NULL;
    }
    SpSearchStructure *data = (SpSearchStructure *)calloc(1, sizeof(SpSearchStructure));
    if (!data) {
        return NULL;
    }
    data->array = (NodeArray *)calloc(1, sizeof(NodeArray));
    if (!data->array) {
        free(data);
        return NULL;
    }
    data->info = info;
    data->max_delta = 0;
    TreeTraversing(tree, Special, data);
    size_t count = data->array->size;
    if (count > 0) {
        data->array->node_array = (Node **)calloc(count, sizeof(Node *));
        if (!data->array->node_array) {
            free(data->array);
            free(data);
            return NULL;
        }
    }
    data->array->size = 0;
    TreeTraversing(tree, AllSpecialNodes, data);
    return data;
}

void SpSearchStructureDelete(SpSearchStructure *data) {
    if (!data) {
        return;
    }
    if (data->array) {
        if (data->array->node_array) {
            free(data->array->node_array);
        }
        free(data->array);
    }
    free(data);
}

void TreeDelete(Tree *tree) {
    if (!tree) {
        return;
    }
    if (tree->root) {
        Stack *stack = StackCreate();
        if (stack) {
            StackPush(stack, tree->root);
            while (!IsEmpty(stack)) {
                Node *cur = StackPop(stack);
                if (cur->relatives[LEFT]) {
                    StackPush(stack, cur->relatives[LEFT]);
                }
                if (cur->relatives[RIGHT]) {
                    StackPush(stack, cur->relatives[RIGHT]);
                }
                NodeDelete(cur);
            }
            StackFree(stack);
        }
    }
    free(tree);
}
