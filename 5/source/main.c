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
    FunctionArray FuncArray[] = {ProgramEnd, DoInsert, DoAddEdge, DoUpdateVertex, DoImport, DoExport, DoDijkstra, DoBFS, DoMST, DoAdjacencyOutput, DoDeleteVertex, DoDeleteEdge, DoGraphviz};
    GraphStatus stat = GRAPH_OK;
    int option = 0;
    InputStatus inp_stat = INPUT_OK;
    while (cur != ProgramEnd && stat != GRAPH_MEMORY_ERROR) {
        menu_print();
        printf("select an option:\n");
        inp_stat = GetInt(&option, 0, 12);
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
        if (((cur == DoAdjacencyOutput) + (cur == DoGraphviz) + (graph->data->size == 0)) == 0) {
            GraphExportDot(graph, "maze.dot", NULL);
            ViewGraph(graph, "maze.dot");
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
    printf("3: update vertex info\n");
    printf("4: graph import\n");
    printf("5: graph export\n");
    printf("6: shortest path dijkstra\n");
    printf("7: bfs path to exit\n");
    printf("8: minimum spanning tree\n");
    printf("9: adjacency output\n");
    printf("10: delete vertex\n");
    printf("11: delete edge\n");
    printf("12: graphviz\n");
}
