#include <stdio.h>
#include <stdlib.h>
#include "dialogue.h"
#include "input.h"
#include "node.h"
#include "tree.h"

TreeStatus DoInsert(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    printf("enter the key:\n");
    size_t key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
        return TREE_END;
    }
    printf("enter the information:\n");
    size_t info_val = 0;
    stat = GetSizeT(&info_val);
    if (stat != INPUT_OK) {
        return TREE_END;
    }
    TreeStatus proc_stat = TreeInsert(tree, key, (void *)&info_val);
    if (proc_stat != TREE_OK) {
        return proc_stat;
    }
    return TREE_OK;
}

TreeStatus DoFindKey(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    printf("enter the search key:\n");
    size_t key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
        return TREE_END;
    }
    NodeArray *res = FindKey(tree, key);
    if (!res || res->size == 0) {
        NodeArrayDelete(res);
        return TREE_NOT_FOUND;
    }
    for (size_t i = 0; i < res->size; i++) {
        printf("\nfound node - key: %zu, info: %zu, release: %zu\n", res->node_array[i]->key, res->node_array[i]->info->info, i + 1);
    }
    NodeArrayDelete(res);
    return TREE_OK;
}

TreeStatus DoFindRelease(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    printf("enter the search key:\n");
    size_t key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
        return TREE_END;
    }
    printf("enter the release:\n");
    size_t release = 0;
    stat = GetSizeT(&release);
    if (stat != INPUT_OK) {
        return TREE_END;
    }
    Node *res = FindKeyRelease(tree, key, release);
    if (!res) {
        return TREE_NOT_FOUND;
    }
    printf("\nfound release - key: %zu, info: %zu\n", res->key, res->info->info);
    return TREE_OK;
}

TreeStatus DoImport(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    char *filename = NULL;
    printf("enter name of the file regarding project root directory:\n");
    InputStatus stat = GetString(&filename);
    if (stat != INPUT_OK) {
        free(filename);
        return TREE_END;
    }
    TreeStatus proc_stat = TreeImport(tree, filename);
    free(filename);
    return proc_stat;
}

TreeStatus DoExport(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    char *filename = NULL;
    printf("enter name of the file regarding project root directory:\n");
    InputStatus stat = GetString(&filename);
    if (stat != INPUT_OK) {
        free(filename);
        return TREE_END;
    }
    TreeStatus proc_stat = TreeExport(tree, filename);
    free(filename);
    if (proc_stat != TREE_OK) {
        return proc_stat;
    }
    return TREE_OK;
}

TreeStatus DoReverseOutput(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    TreeStatus stat = TreeTraversing(tree, Output,  NULL);
    if (stat != TREE_OK) {
        return stat;
    }
    return TREE_OK;
}

TreeStatus DoSpecialSearch(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    printf("enter the search info for max delta:\n");
    size_t info = 0;
    InputStatus stat = GetSizeT(&info);
    if (stat != INPUT_OK) {
        return TREE_END;
    }
    SpSearchStructure *res = SpecialSearch(tree, info);
    if (!res || !res->array || res->array->size == 0) {
        SpSearchStructureDelete(res);
        return TREE_NOT_FOUND;
    }
    printf("\nmax delta: %zu\n", res->max_delta);
    for (size_t i = 0; i < res->array->size; i++) {
        printf("special Node[%zu] - key: %zu, info: %zu\n", i, res->array->node_array[i]->key, res->array->node_array[i]->info->info);
    }
    SpSearchStructureDelete(res);
    return TREE_OK;
}

TreeStatus DoGraphviz(Tree *tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    if (!tree->root) {
        return TREE_EMPTY;
    }
    char *filename = NULL;
    printf("enter name of the file regarding project root directory:\n");
    InputStatus stat = GetString(&filename);
    if (stat != INPUT_OK) {
        free(filename);
        return TREE_END;
    }
    TreeExportDot(tree, filename);
    char command[1024] = {};
    sprintf(command, "dot -Tpng %s -o image/tree.png && kitten icat image/tree.png", filename);
    system(command);
    free(filename);
    return TREE_OK;
}

TreeStatus DoDeleteKey(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    printf("enter deleting key:\n");
    size_t key = 0;
    InputStatus stat = GetSizeT(&key);
    if (stat != INPUT_OK) {
        return TREE_END;
    }
    TreeStatus proc_stat = TreeKeyDelete(tree, key);
    if (proc_stat != TREE_OK) {
        return proc_stat;
    }
    return TREE_OK;
}

TreeStatus DoOutput(Tree *const tree) {
    if (!tree) {
        return TREE_NOT_VALID;
    }
    int choice = 0;
    printf("\n1 - info + key, 2 - key, 3 - info\n");
    InputStatus inp_stat = GetInt(&choice, 1, 3);
    if (inp_stat != INPUT_OK) {
        return TREE_END;
    }
    OutputType array[] = {NodeToString, NodeKeyToString, NodeInfoToString};
    OutputType cur_way = array[choice - 1];
    TreeStatus stat = TreeOutput(tree, cur_way);
    if (stat != TREE_OK) {
        return stat;
    }
    return TREE_OK;
}

TreeStatus ProgramEnd(Tree *const tree) {
    (void)tree;
    return TREE_OK;
}
