#include "table.h"
#include "input.h"
#include "dialogue.h"
#include <stdio.h>

void menu_print();

int main() {
    Table *table = TableCreate();
    char *conditions[] = {"OK", "NOT_VALID", "NOT_FOUND", "TABLE_EMPTY", "MEMORY_ERROR", \
        "RELEASE DUPLICATE", "WRONG FORMAT", "END OF INPUT"};
    FunctionArray cur = NULL;
    FunctionArray FuncArray[] = {DoInsert, DoFindVersion, DoFindKey, DoImport, DoExport, DoDeleteVersion, DoDeleteKey, DoOutput, ProgramEnd};
    TableStatus stat = OK;
    int option = 0;
    InputStatus inp_stat = INPUT_OK;
    while (cur != ProgramEnd && stat != MEMORY_ERROR && stat != NOT_VALID && stat != END_OF_INPUT) {
        menu_print();
        printf("select an option\n");
        inp_stat = GetInt(&option, 1, 9);
        if (inp_stat != INPUT_OK) {
            stat = END_OF_INPUT;
            printf("\nINPUT END\n\n");
            break;
        }
        cur = FuncArray[option - 1];
        stat = cur(table);
        printf("\n%s\n", conditions[stat]);
        TableOutput(table);
        printf("\n");
    }
    TableDelete(table);
    return 0;
}

void menu_print() {
    printf("1: element insert\n");
    printf("2: find element by key and version\n");
    printf("3: find all key releases\n");
    printf("4: import information from text file\n");
    printf("5: export information to text file\n");
    printf("6: delete key version\n");
    printf("7: delete all key releases\n");
    printf("8: table output\n");
    printf("9: program end\n");
}
