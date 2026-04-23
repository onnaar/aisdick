#include <stdio.h>
#include <stdlib.h>
#include "dialogue.h"
#include "input.h"
#include "tree.h"

TreeStatus DoInsert(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_EXIST;
    }
    printf("enter the key:\n");
    size_t key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
        return TREE_OK;
    }
    printf("enter the information:\n");
    size_t info_val = 0;
    stat = GetSizeT(&info_val);
    if (stat != INPUT_OK) {
        return TREE_OK;
    }
    TreeStatus proc_stat = TreeInsert(tree, key, &info_val);
    if (proc_stat != TREE_OK) {
        return proc_stat;
    }
    return TREE_OK;
}

TreeStatus DoFindKey(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_EXIST;
    }
    printf("enter the search key:\n");
    size_t key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
        return TREE_OK;
    }
    Node *res = FindKey(tree, key);
    if (!res) {
        return TREE_NOT_FOUND;
    }
    printf("Found Node - Key: %zu, Info: %zu\n", res->key, *(res->info));
    return TREE_OK;
}

TreeStatus DoFindRelease(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_EXIST;
    }
    printf("enter the search key:\n");
    size_t key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
        return TREE_OK;
    }
    printf("enter the release:\n");
    size_t release = 0;
    stat = GetSizeT(&release);
    if (stat != INPUT_OK) {
        return TREE_OK;
    }
    NodeArray *res = FindKeyRelease(tree, key, release);
    if (!res) {
        return TREE_NOT_FOUND;
    }
    printf("Found Release - Key: %zu, Info: %zu\n", res->node_array[0]->key, *(res->node_array[0]->info));
    NodeArrayManage(res, 0); 
    return TREE_OK;
}

TreeStatus DoSpecialSearch(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_EXIST;
    }
    printf("enter the search info for max delta:\n");
    size_t info = 0;
    InputStatus stat = GetSizeT(&info);
    if (stat != INPUT_OK) {
        return TREE_OK;
    }
    SpSearchStructure *res = SpecialSearch(tree, info);
    if (!res || !res->array || res->array->size == 0) {
        SpSearchStructureDelete(res);
        return TREE_NOT_FOUND;
    }
    printf("Max delta: %zu\n", res->max_delta);
    for (size_t i = 0; i < res->array->size; i++) {
        printf("Special Node[%zu] - Key: %zu, Info: %zu\n", i, res->array->node_array[i]->key, *(res->array->node_array[i]->info));
    }
    SpSearchStructureDelete(res);
    return TREE_OK;
}

TreeStatus DoDeleteKey(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_EXIST;
    }
    printf("enter deleting key:\n");
    size_t key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
        return TREE_OK;
    }
    TreeStatus proc_stat = TreeKeyDelete(tree, key);
    if (proc_stat != TREE_OK) {
        return proc_stat;
    }
    return TREE_OK;
}

TreeStatus DoOutput(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_EXIST;
    }
    TreeStatus stat = TreeTraversing(tree, Output, NULL);
    if (stat != TREE_OK) {
        return stat;
    }
    return TREE_OK;
}

TreeStatus ProgramEnd(Tree *const tree) {
    (void)tree;
    return TREE_OK;
}
