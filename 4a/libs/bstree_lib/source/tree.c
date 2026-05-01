#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "tree.h"
#include "node.h"
#include "stack.h"

#define MAGIC_WORD "TREE_STRUCTURE"

// │ 2502 ├ 251c └ 2514 ─ 2500

Tree *TreeCreate() {
     return (Tree *)calloc(1, sizeof(Tree));
}

TreeStatus TreeInsert(Tree *tree, size_t key, NodeInfo *info) {
    if (!tree || !info) {
        return TREE_NOT_VALID;
    }
    Node *cur = tree->root, *prev = NULL;
    RelativeIndex index = LEFT;
    while (cur) {
        prev = cur;
        index = (key >= cur->key) ? RIGHT : LEFT;
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
    prev->relatives[index] = temp;
    return TREE_OK;
}

NodeArray *FindKey(Tree *tree, size_t key) {
    if (!tree || !tree->root) {
        return NULL;
    }
    NodeArray *array = NodeArrayCreate();
    if (!array) {
        return NULL;
    }
    Node *cur = tree->root;
    RelativeIndex index = LEFT;
    while (cur) {
        index = LEFT;
        if (key >= cur->key) {
            index = RIGHT;
        }
        if (cur->key == key) {
            NodeArrayAdd(array, cur);
        }
        cur = cur->relatives[index];
    }
    return array;
}

TreeStatus TreeKeyDelete(Tree *tree, size_t key) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    Node *target = FindKeyRelease(tree, key, 1);
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
    if (target->info && successor->info) {
        *target->info = *successor->info;
    }
    index = ((successor->relatives[PARENT]->relatives[LEFT] == successor) ? LEFT : RIGHT);
    successor->relatives[PARENT]->relatives[index] = successor->relatives[RIGHT];
    if (successor->relatives[RIGHT]) {
        successor->relatives[RIGHT]->relatives[PARENT] = successor->relatives[PARENT];
    }
    NodeDelete(successor);
    return TREE_OK;
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

Node *FindKeyRelease(Tree *tree, size_t key, size_t release) {
    if (!tree) {
        return NULL;
    }
    NodeArray *array = FindKey(tree, key);
    if (!array) {
        return NULL;
    }
    if (release > array->size) {
        return NULL;
    }
    Node *res = array->node_array[release - 1];
    NodeArrayDelete(array);
    return res;
}

TreeStatus TreeTraversing(Tree *tree, void (*action)(Node *cur, void *context), void *context) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    if (!tree->root) {
        return TREE_EMPTY;
    }
    Stack *stack = StackCreate();
    if (!stack) {
        return TREE_MEMORY_ERROR;
    }
    Node *cur = tree->root, *left_node = NULL;
    while (cur || !IsEmpty(stack)) {
        while (cur) {
            StackPush(stack, cur);
            cur = cur->relatives[RIGHT];
        }
        cur = StackPop(stack);
        left_node = cur->relatives[LEFT];
        action(cur, context);
        cur = left_node;
    }
    StackFree(stack);
    return TREE_OK;    
}

void Special(Node *cur, void *context) {
    SpSearchStructure *data = (SpSearchStructure *)context; 
    size_t delta = (cur->key > data->key) ? (cur->key - data->key) : (data->key - cur->key);
    if (delta > data->max_delta) {
        data->max_delta = delta;
    }
}

void AllSpecialNodes(Node *cur, void *context) {
    SpSearchStructure *data = (SpSearchStructure *)context; 
    size_t delta = (cur->key > data->key) ? (cur->key - data->key) : (data->key - cur->key);
    if (delta == data->max_delta) {
        NodeArrayAdd(data->array, cur);
    }
}

void Delete(Node *cur, void *context) {
    if (!cur) {
        return;
    }
    (void)context;
    NodeDelete(cur);
}

SpSearchStructure *SpecialSearch(Tree *tree, size_t key) {
    if (!tree || !tree->root) {
        return NULL;
    }
    SpSearchStructure *data = SpSearchStructureCreate();
    if (!data) {
        return NULL;
    }
    data->array = NodeArrayCreate();
    if (!data->array) {
        free(data);
        return NULL;
    }
    data->key = key;
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

SpSearchStructure *SpSearchStructureCreate() {
    return (SpSearchStructure *)calloc(1, sizeof(SpSearchStructure));
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

TreeStatus TreeImport(Tree *const tree, const char *const filename) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    FILE *file = fopen(filename, "r");
    if (!file) {
        return TREE_NOT_FOUND;
    }
    size_t line_number = 1;
    char *magic = my_readline(file);
    if (!magic || (strcmp(magic, MAGIC_WORD)) != 0) {
        if (magic) {
            free(magic);
        }
        fclose(file);
        return TREE_WRONG_FORMAT;
    }
    free(magic);
    char *buffer = NULL;
    while ((buffer = my_readline(file))) {
        line_number++;
        if (strlen(buffer) <= 1 && *buffer == '\n') {
            continue;
        }
        size_t key = 0;
        if (StrToZu(buffer, &key) != INPUT_OK) {
            printf("\nerror format (key): line %zu\n", line_number);
            free(buffer);
            continue;
        }
        free(buffer);
        line_number++;
        buffer = my_readline(file);
        size_t info_val = 0;
        if (StrToZu(buffer, &info_val) != INPUT_OK) {
            printf("\nerror format (info): line %zu\n", line_number);
            free(buffer);
            continue;
        }
        NodeInfo *info_ptr = (NodeInfo *)calloc(1, sizeof(NodeInfo));
        if (info_ptr) {
            info_ptr->info = info_val;
            TreeStatus stat = TreeInsert(tree, key, info_ptr);
            if (stat == TREE_DUPLICATE) {
                printf("\nkey %zu - duplicate, line %zu\n", key, line_number - 1);
            }
            free(info_ptr);
        }
        free(buffer);
    }
    fclose(file);
    return TREE_OK;
}

TreeStatus TreeExport(Tree *tree, const char *filename) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        return TREE_NOT_FOUND;
    }
    fprintf(file, "%s\n", MAGIC_WORD);
    Stack *stack = StackCreate();
    if (tree->root) {
        StackPush(stack, tree->root);
    }
    while (!IsEmpty(stack)) {
        Node *cur = (Node *)StackPop(stack);
        fprintf(file, "%zu\n%zu\n", cur->key, cur->info->info);
        if (cur->relatives[RIGHT]) {
            StackPush(stack, cur->relatives[RIGHT]);
        }
        if (cur->relatives[LEFT]) {
            StackPush(stack, cur->relatives[LEFT]);
        }
    }
    StackFree(stack);
    fclose(file);
    return TREE_OK;
}

static void NewChildItem(Stack *stack, PrintStackItem *current_item, Node *child_node, bool is_last, RelativeIndex index) {
    if (!child_node) {
        return;
    }
    PrintStackItem *child_item = (PrintStackItem *)calloc(1, sizeof(PrintStackItem));
    if (child_item) {
        child_item->index = (index == LEFT) ? LEFT : RIGHT;
        child_item->node = child_node;
        child_item->depth = current_item->depth + 1;
        child_item->is_last_child = is_last;
        for (size_t i = 1; i < current_item->depth; i++) {
            child_item->level_history[i] = current_item->level_history[i];
        }
        if (current_item->depth < MAX_PRINT_DEPTH) {
            child_item->level_history[current_item->depth] = current_item->is_last_child;
        }
        StackPush(stack, child_item);
    }
}

TreeStatus TreeOutput(Tree *tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    if (!tree->root) {
        return TREE_EMPTY;
    }
    Stack *stack = StackCreate();
    if (!stack) {
        return TREE_MEMORY_ERROR;
    }
    PrintStackItem *root_item = (PrintStackItem *)calloc(1, sizeof(PrintStackItem));
    if (!root_item) {
        StackFree(stack);
        return TREE_MEMORY_ERROR;
    }
    root_item->node = tree->root;
    root_item->is_last_child = true;
    StackPush(stack, root_item);
    while (!IsEmpty(stack)) {
        PrintStackItem *current_item = (PrintStackItem *)StackPop(stack);
        Node *current_node = current_item->node;
        size_t current_depth = current_item->depth;
        bool is_last_child = current_item->is_last_child;
        if (current_depth > 0) {
            for (size_t i = 1; i < current_depth; i++) {
                if (current_item->level_history[i]) {
                    printf("\t");
                } else {
                    printf("│   ");
                }
            }
            char *description = (current_item->index == LEFT) ? "left" : "right";
            printf("%s%s", is_last_child ? "└── " : "├── ", description);
        } else {
            printf("root ");
        }
        printf("[%zu:%zu]\n", current_node->key, current_node->info->info);
        Node *left_node = current_node->relatives[LEFT];
        Node *right_node = current_node->relatives[RIGHT];
        NewChildItem(stack, current_item, left_node, true, LEFT);
        NewChildItem(stack, current_item, right_node, left_node == NULL, RIGHT);
        free(current_item);
    }
    StackFree(stack);
    return TREE_OK;
}

static void LinkCreate(Stack *stack, FILE *file, Node *node, RelativeIndex index) {
    if (node->relatives[index]) {
        char *color = (index == RIGHT) ? "blue" : "red";
        char *label = (index == RIGHT) ? "R" : "L";
        fprintf(file, "\tn_%p -> n_%p [label=\" %s\", color=\"%s\"];\n", node, node->relatives[index], label, color);
        StackPush(stack, node->relatives[index]);
    } else {
        char side = (index == RIGHT) ? 'r' : 'l';
        fprintf(file, "\t%c_%p [style=invis, label=\"\", width=0, height=0];\n", side, node);
        fprintf(file, "\tn_%p -> %c_%p [style=invis];\n", node, side, node);
    }
}

TreeStatus TreeExportDot(Tree *tree, const char *filename) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    if (!tree->root) {
        return TREE_EMPTY;
    }
    Stack *stack = StackCreate();
    if (!stack) {
        return TREE_MEMORY_ERROR;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        return TREE_NOT_FOUND;
    }
    fprintf(file, "digraph G {\n");
    fprintf(file, "\tgraph [rankdir=LR, splines = line, dpi=150];\n");
    fprintf(file, "\tnode [shape=box, style=rounded];\n");
    StackPush(stack, tree->root);
    while (!IsEmpty(stack)) {
        Node *node = (Node *)StackPop(stack);
        fprintf(file, "\tn_%p [label=\"ключ:%zu\\nзнач:%zu\"];\n", node, node->key, node->info->info);
        LinkCreate(stack, file, node, RIGHT);
        LinkCreate(stack, file, node, LEFT);
    }
    fprintf(file, "}\n");
    StackFree(stack);
    fclose(file);
    return TREE_OK;
}
