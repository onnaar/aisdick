#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "dialogue.h"
#include "graph.h"
#include "input.h"
#include "vertex.h"

GraphStatus DoInsert(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    printf("enter the coordinates (x and y):\n");
    size_t x = 0, y = 0;
    if (GetSizeT(&x) != INPUT_OK || GetSizeT(&y) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    Point coords = {x, y};
    printf("enter the vertex type (-1 - EXIT, 0 - TRANSITION, 1 - ENTRANCE):\n");
    int type_choice = 0;
    if (GetInt(&type_choice, -1, 1) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    GraphStatus proc_stat = GraphAddVertex(graph, coords, (VertexType)type_choice);
    if (proc_stat != GRAPH_OK) {
        return proc_stat;
    }
    return GRAPH_OK;
}

GraphStatus DoAddEdge(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    printf("enter the source coordinates (x and y):\n");
    size_t x = 0, y = 0;
    if (GetSizeT(&x) != INPUT_OK || GetSizeT(&y) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    Point from_coords = {x, y};
    printf("enter the direction (0 - UP, 1 - RIGHT, 2 - DOWN, 3 - LEFT):\n");
    int dir_choice = 0;
    if (GetInt(&dir_choice, 0, 3) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    GraphStatus proc_stat = GraphAddEdge(graph, from_coords, (Neighbours)dir_choice);
    if (proc_stat != GRAPH_OK) {
        return proc_stat;
    }
    return GRAPH_OK;
}

GraphStatus DoDeleteVertex(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    printf("enter target coordinates (x and y):\n");
    size_t x = 0, y = 0;
    if (GetSizeT(&x) != INPUT_OK || GetSizeT(&y) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    Point target_coords = {x, y};
    GraphStatus proc_stat = GraphRemoveVertex(graph, target_coords);
    if (proc_stat != GRAPH_OK) {
        return proc_stat;
    }
    return GRAPH_OK;
}

GraphStatus DoUpdateVertex(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    printf("enter target vertex id:\n");
    size_t target_id = 0;
    if (GetSizeT(&target_id) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    printf("enter new coordinates (x and y):\n");
    size_t x = 0, y = 0;
    if (GetSizeT(&x) != INPUT_OK || GetSizeT(&y) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    Point new_coords = {x, y};
    GraphStatus proc_stat = GraphUpdateVertexByID(graph, target_id, new_coords);
    if (proc_stat != GRAPH_OK) {
        return proc_stat;
    }
    return GRAPH_OK;
}

GraphStatus DoImport(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    char *filename = NULL;
    printf("enter name of the file regarding project root directory:\n");
    InputStatus stat = GetString(&filename);
    if (stat != INPUT_OK) {
        free(filename);
        return GRAPH_END_OF_INPUT;
    }
    GraphStatus proc_stat = GraphImport(graph, filename);
    free(filename);
    return proc_stat;
}

GraphStatus DoExport(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    char *filename = NULL;
    printf("enter name of the file regarding project root directory:\n");
    InputStatus stat = GetString(&filename);
    if (stat != INPUT_OK) {
        free(filename);
        return GRAPH_END_OF_INPUT;
    }
    GraphStatus proc_stat = GraphExport(graph, filename);
    free(filename);
    if (proc_stat != GRAPH_OK) {
        return proc_stat;
    }
    return GRAPH_OK;
}

GraphStatus DoAdjacencyOutput(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    GraphStatus stat = GraphAdjacencyOutput(graph);
    if (stat != GRAPH_OK) {
        return stat;
    }
    return GRAPH_OK;
}

GraphStatus DoDijkstra(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    printf("enter start vertex id:\n");
    size_t start_id = 0;
    if (GetSizeT(&start_id) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    printf("enter finish vertex id:\n");
    size_t finish_id = 0;
    if (GetSizeT(&finish_id) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    Vertex **path = ShortestPathDijkstra(graph, start_id, finish_id);
    if (!path) {
        return GRAPH_NOT_FOUND;
    }
    printf("\nshortest path found:\n");
    for (size_t i = 0; path[i] != NULL; i++) {
        printf("step [%zu] -> node ID: %zu, coords: (%zu, %zu)\n", i, path[i]->id, path[i]->coords.x, path[i]->coords.y);
    }
    free(path);
    return GRAPH_OK;
}

GraphStatus DoBFS(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    printf("enter start vertex id:\n");
    size_t start_id = 0;
    if (GetSizeT(&start_id) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    Vertex **path = BFS(graph, start_id);
    if (!path) {
        return GRAPH_NOT_FOUND;
    }
    printf("\nbfs path to exit found:\n");
    for (size_t i = 0; path[i] != NULL; i++) {
        printf("node ID: %zu, coords: (%zu, %zu)\n", path[i]->id, path[i]->coords.x, path[i]->coords.y);
    }
    free(path);
    return GRAPH_OK;
}

GraphStatus DoGraphviz(Graph *const graph) {
    (void)graph;
    return GRAPH_OK;
}

GraphStatus DoSpecialSearch(Graph *const graph) {
    (void)graph;
    return GRAPH_OK;
}

GraphStatus ProgramEnd(Graph *const graph) {
    (void)graph;
    return GRAPH_OK;
}

