#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <graphviz/gvc.h>
#include "input.h"
#include "tree.h"
#include "stack.h"

#define MAGIC_WORD "TREE_STRUCTURE"

// │ 2502 ├ 251c └ 2514 ─ 2500

Tree *TreeCreate() {
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
        return TREE_NOT_VALID;
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
        return TREE_NOT_VALID;
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

void Delete(Node *cur, void *context) {
    if (!cur) {
        return;
    }
    (void)context;
    NodeDelete(cur);
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
                if (current_item->line_history[i]) {
                    printf("\t");
                } else {
                    printf("│   ");
                }
            }
            printf("%s", is_last_child ? "└── " : "├── ");
        }
        printf("[%zu:%zu]\n", current_node->key, *(current_node->info));
        Node *left_node = current_node->relatives[LEFT];
        Node *right_node = current_node->relatives[RIGHT];
        if (right_node) {
            PrintStackItem *right_item = (PrintStackItem *)calloc(1, sizeof(PrintStackItem));
            if (right_item) {
                right_item->node = right_node;
                right_item->depth = current_depth + 1;
                right_item->is_last_child = true;
                for (size_t i = 1; i < current_depth; i++) {
                    right_item->line_history[i] = current_item->line_history[i];
                }
                if (current_depth < MAX_PRINT_DEPTH) {
                    right_item->line_history[current_depth] = is_last_child;
                }
                StackPush(stack, right_item);
            }
        }
        // additional func to make it easier
        if (left_node) {
            PrintStackItem *left_item = (PrintStackItem *)calloc(1, sizeof(PrintStackItem));
            if (left_item) {
                left_item->node = left_node;
                left_item->depth = current_depth + 1;
                left_item->is_last_child = (right_node == NULL) ? true : false;
                for (size_t i = 1; i < current_depth; i++) {
                    left_item->line_history[i] = current_item->line_history[i];
                }
                if (current_depth < MAX_PRINT_DEPTH) {
                    left_item->line_history[current_depth] = is_last_child;
                }
                StackPush(stack, left_item);
            }
        }
        /*for (int i = 0; i < MAX_PRINT_DEPTH; i++) {
            printf("%d ", current_item->line_history[i]);
        }
        printf("\n");*/
        free(current_item);
    }
    StackFree(stack);
    return TREE_OK;
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
        size_t *info_ptr = (size_t *)calloc(1, sizeof(size_t));
        if (info_ptr) {
            *info_ptr = info_val;
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

static void node_export(Node *node, void *context) {
    if (!node) {
        return;
    }
    FILE *file = (FILE *)context;
    // Записываем ключ и значение на разных строках, как требует формат
    fprintf(file, "%zu\n%zu\n", node->key, *(node->info));
}

TreeStatus TreeExport(Tree *tree, const char *filename) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        return TREE_NOT_FOUND;
    }
    // Записываем магическое слово
    fprintf(file, "%s\n", MAGIC_WORD);
    // Нам нужен Pre-order, чтобы дерево восстановилось 1 в 1.
    // Если твой универсальный обход это умеет — используй его.
    // Если нет — вот простой итеративный Pre-order на стеке:
    Stack *stack = StackCreate();
    if (tree->root) {
        StackPush(stack, tree->root);
    }
    while (!IsEmpty(stack)) {
        Node *cur = (Node *)StackPop(stack);
        node_export(cur, file);
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

static void node_to_gvc_record(Node *node, void *context) {
    if (!node || !context) {
        return;
    }
    Agraph_t *graph = (Agraph_t *)context;
    char parent_name[32];
    sprintf(parent_name, "%zu", node->key);
    Agnode_t *parent_gnode = agnode(graph, parent_name, 1);
    char label[128];
    sprintf(label, "{<k>%zu|{<l>|<r>}}", node->key);
    agsafeset(parent_gnode, "label", label, "");
    if (node->relatives[LEFT]) {
        char left_name[32];
        sprintf(left_name, "%zu", node->relatives[LEFT]->key);
        Agnode_t *left_gnode = agnode(graph, left_name, 1);
        Agedge_t *edge = agedge(graph, parent_gnode, left_gnode, 0, 1);
        agsafeset(edge, "tailport", "l", "");
    }
    if (node->relatives[RIGHT]) {
        char right_name[32];
        sprintf(right_name, "%zu", node->relatives[RIGHT]->key);
        Agnode_t *right_gnode = agnode(graph, right_name, 1);
        Agedge_t *edge = agedge(graph, parent_gnode, right_gnode, 0, 1);
        agsafeset(edge, "tailport", "r", "");
    }
}

TreeStatus TreeGraphviz(Tree *tree, const char *filename) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    if (!tree->root) {
        return TREE_EMPTY;
    }
    GVC_t *gvc = gvContext();
    if (!gvc) {
        return TREE_MEMORY_ERROR;
    }
    Agraph_t *graph = agopen("Tree", Agdirected, 0);
    if (!graph) {
        gvFreeContext(gvc);
        return TREE_MEMORY_ERROR;
    }
    agattr(graph, AGNODE, "shape", "record");
    agattr(graph, AGNODE, "fontname", "Arial");
    TreeTraversing(tree, node_to_gvc_record, graph);
    FILE *file = fopen(filename, "w");
    if (!file) {
        agclose(graph);
        gvFreeContext(gvc);
        return TREE_NOT_FOUND;
    }
    gvLayout(gvc, graph, "dot");
    gvRender(gvc, graph, "png", file);
    fclose(file);
    gvFreeLayout(gvc, graph);
    agclose(graph);
    gvFreeContext(gvc);
    return TREE_OK;
}
