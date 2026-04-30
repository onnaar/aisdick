#include <stdio.h>
#include <stdlib.h>
#include "dialogue.h"
#include "input.h"
#include "tree.h"

void menu_print();

int main() {
    Tree *tree = TreeCreate();
    if (!tree) {
        return 666;
    }
    char *conditions[] = {"OK", "NOT VALID", "MEMORY ERROR", "NOT_FOUND", "TREE EMPTY", "INPUT END", "WRONG FORMAT"};
    FunctionArray cur = NULL;
    FunctionArray FuncArray[] = {ProgramEnd, DoInsert, DoFindKey, DoFindRelease, DoImport, DoExport, DoSpecialSearch, DoGraphviz, DoDeleteKey, DoOutput};
    TreeStatus stat = TREE_OK;
    int option = 0;
    InputStatus inp_stat = INPUT_OK;
    while (cur != ProgramEnd && stat != TREE_MEMORY_ERROR) {
        menu_print();
        printf("select an option:\n");
        inp_stat = GetInt(&option, 0, 9);
        if (inp_stat != INPUT_OK) {
            printf("\nINPUT END\n\n");
            break;
        }
        cur = FuncArray[option];
        stat = cur(tree);
        printf("\n%s\n\n", conditions[stat]);
        if (stat == TREE_END) {
            break;
        }
        if (cur != DoGraphviz) {
            TreeOutput(tree);
        }
        printf("\n");
    }
    TreeTraversing(tree, Delete, NULL);
    free(tree);
    return 0;
}

void menu_print() {
    printf("0: program end\n");
    printf("1: element insert\n");
    printf("2: find element by key\n");
    printf("3: find key release (version)\n");
    printf("4: tree import\n");
    printf("5: tree export\n");
    printf("6: special search (max delta)\n");
    printf("7: tree graphviz\n");
    printf("8: delete key\n");
    printf("9: tree output\n");
}
