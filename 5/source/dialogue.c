#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include "dialogue.h"
#include "graph.h"
#include "input.h"
#include "vertex.h"

void ViewGraph(const Graph *const graph, const char *const dot_filename) {
    if (!graph || graph->vertex_counter == 0) {
        return;
    }
    char command[1024] = {};
    sprintf(command, "neato -Gdpi=300 -Tpng %s -o image/maze.png && kitten icat image/maze.png", dot_filename);
    system(command);
}

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
    return GraphAddVertex(graph, coords, (VertexType)type_choice);
}

GraphStatus DoAddEdge(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    printf("enter the id:\n");
    size_t id = 0;
    if (GetSizeT(&id) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    printf("enter the direction (0 - UP, 1 - RIGHT, 2 - DOWN, 3 - LEFT):\n");
    int dir_choice = 0;
    if (GetInt(&dir_choice, 0, 3) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    return GraphAddEdge(graph, id, (Neighbours)dir_choice);
}

GraphStatus DoUpdateVertex(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    printf("enter target vertex id:\n");
    size_t target_id = 0, x = 0, y = 0;
    int type = 0;
    if (GetSizeT(&target_id) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    printf("enter new coordinates (x && y):\n");
    if (GetSizeT(&x) != INPUT_OK || GetSizeT(&y) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    printf("new type of vertex (0 - EXIT, 1 - TRANSITION, 2 - ENTRANCE):\n");
    if (GetInt(&type, 0, 2) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    Point new_coords = {x, y};
    return GraphUpdateVertex(graph, target_id, new_coords, (VertexType)type - 1);
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
    return proc_stat;
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
    GraphExportDot(graph, "maze.dot", path);
    ViewGraph(graph, "maze.dot");
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
    GraphExportDot(graph, "maze.dot", path);
    ViewGraph(graph, "maze.dot");
    free(path);
    return GRAPH_OK;
}

GraphStatus DoMST(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    return GraphMakeMST(graph);
}

GraphStatus DoAdjacencyOutput(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    return GraphAdjacencyOutput(graph);
}

GraphStatus DoDeleteVertex(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    printf("enter target id:\n");
    size_t id = 0;
    if (GetSizeT(&id) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    return GraphRemoveVertex(graph, id);
}

GraphStatus DoDeleteEdge(Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    printf("enter the id:\n");
    size_t id = 0;
    if (GetSizeT(&id) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    printf("enter the direction (0 - UP, 1 - RIGHT, 2 - DOWN, 3 - LEFT):\n");
    int dir_choice = 0;
    if (GetInt(&dir_choice, 0, 3) != INPUT_OK) {
        return GRAPH_END_OF_INPUT;
    }
    return GraphRemoveEdge(graph, id, (Neighbours)dir_choice);
}

GraphStatus DoGraphviz(Graph *const graph) {
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
    GraphStatus proc_stat = GraphExportDot(graph, filename, NULL);
    if (proc_stat != GRAPH_OK) {
        free(filename);
        return proc_stat;
    }
    ViewGraph(graph, filename);
    free(filename);
    return GRAPH_OK;
}

GraphStatus ProgramEnd(Graph *const graph) {
    (void)graph;
    return GRAPH_OK;
}
