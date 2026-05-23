#include <stdio.h>
#include <stdlib.h>
#include "dialogue.h"
#include "graph.h"
#include "input.h"

void menu_print();

int main() {
    Graph *graph = GraphCreate();
    if (!graph) {
        return 666;
    }
    char *conditions[] = {"OK", "NOT VALID", "NOT FOUND", "GRAPH EMPTY", "MEMORY ERROR", "WRONG FORMAT", "INPUT END", "DUPLICATE"};
    FunctionArray cur = NULL;
    FunctionArray FuncArray[] = {ProgramEnd, DoInsert, DoAddEdge, DoDeleteVertex, DoUpdateVertex, DoImport, DoExport, DoAdjacencyOutput, DoDijkstra, DoBFS, DoSpecialSearch, DoGraphviz};
    GraphStatus stat = GRAPH_OK;
    int option = 0;
    InputStatus inp_stat = INPUT_OK;
    while (cur != ProgramEnd && stat != GRAPH_MEMORY_ERROR) {
        menu_print();
        printf("select an option:\n");
        inp_stat = GetInt(&option, 0, 11);
        if (inp_stat != INPUT_OK) {
            printf("\nINPUT END\n\n");
            break;
        }
        cur = FuncArray[option];
        stat = cur(graph);
        printf("\n%s\n\n", conditions[stat]);
        if (stat == GRAPH_END_OF_INPUT) {
            break;
        }
        if (((cur == DoGraphviz) + (cur == DoAdjacencyOutput)) == 0) {
            GraphAdjacencyOutput(graph);
        }
        printf("\n");
    }
    GraphFree(graph);
    return 0;
}

void menu_print() {
    printf("0: program end\n");
    printf("1: element insert\n");
    printf("2: add edge\n");
    printf("3: delete node\n");
    printf("4: update node by ID\n");
    printf("5: graph import\n");
    printf("6: graph export\n");
    printf("7: adjacency output\n");
    printf("8: shortest path dijkstra\n");
    printf("9: bfs path to exit\n");
    printf("10: special search\n");
    printf("11: graph graphviz\n");
}

