#include <stdio.h>
#include "dialogue.h"
#include "input.h"
#include "tree.h"

void menu_print();

int main() {
    Tree *tree = TreeCreate();
    if (!tree) {
        return 666;
    }
    char *conditions[] = {"OK", "NOT VALID", "MEMORY ERROR", "NOT_FOUND", "TREE EMPTY"};
    FunctionArray cur = NULL;
    FunctionArray FuncArray[] = {DoInsert, DoFindKey, DoFindRelease, DoSpecialSearch, DoDeleteKey, DoOutput, ProgramEnd};
    TreeStatus stat = TREE_OK;
    int option = 0;
    InputStatus inp_stat = INPUT_OK;
    while (cur != ProgramEnd && stat != TREE_MEMORY_ERROR) {
        menu_print();
        printf("select an option:\n");
        inp_stat = GetInt(&option, 1, 7);
        if (inp_stat != INPUT_OK) {
            printf("\nINPUT END\n\n");
            break;
        }
        cur = FuncArray[option - 1];
        stat = cur(tree);
        printf("\n%s\n", conditions[stat]);
        printf("\n");
        if (DoOutput(tree) != TREE_EMPTY) {
            printf("\n");
        }
    }
    TreeDelete(tree);
    return 0;
}

void menu_print() {
    printf("1: element insert\n");
    printf("2: find element by key\n");
    printf("3: find key release (version)\n");
    printf("4: special search (max delta)\n");
    printf("5: delete key\n");
    printf("6: tree output\n");
    printf("7: program end\n");
}
