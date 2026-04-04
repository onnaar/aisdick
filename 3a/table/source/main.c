#include "table.h"
#include "input.h"
#include "dialogue.h"
#include <stdio.h>

void menu_print();

int main() {
    Table *table = TableCreate();
    char *conditions[] = {"OK", "NOT_EXIST", "NOT_FOUND", "TABLE_EMPTY", "MEMORY_ERROR", "END OF INPUT"};
    FunctionArray cur = NULL;
    FunctionArray FuncArray[] = {DoInsert, DoFindVersion, DoFindKey, DoImport, DoDeleteVersion, DoDeleteKey, DoOutput, ProgramEnd};
    status stat = OK;
    int option = 0;
    InputStatus inp_stat = INPUT_OK;
    while (cur != ProgramEnd && stat != MEMORY_ERROR && stat != NOT_EXIST && stat != END_OF_INPUT) {
        menu_print();
        printf("select an option\n");
        inp_stat = GetInt(&option, 1, 8);
        if (inp_stat != INPUT_OK) {
            stat = END_OF_INPUT;
            continue;
        }
        cur = FuncArray[option - 1];
        stat = cur(table);
        printf("\n%s\n\n", conditions[stat]);
    }
    TableDelete(table);
    return 0;
}

void menu_print() {
    printf("1: element insert\n");
    printf("2: find element by key and version\n");
    printf("3: find all key releases\n");
    printf("4: import information from text file\n");
    printf("5: delete key version\n");
    printf("6: delete all key releases\n");
    printf("7: table output\n");
    printf("8: program end\n");
}
