#ifndef DIALOGUE_GRAPH_H
#define DIALOGUE_GRAPH_H

#include <stddef.h>
#include "graph.h"

typedef GraphStatus (*FunctionArray)(Graph *const);

GraphStatus DoInsert(Graph *const graph);
GraphStatus DoAddEdge(Graph *const graph);
GraphStatus DoDeleteVertex(Graph *const graph);
GraphStatus DoUpdateVertex(Graph *const graph);
GraphStatus DoImport(Graph *const graph);
GraphStatus DoExport(Graph *const graph);
GraphStatus DoAdjacencyOutput(Graph *const graph);
GraphStatus DoDijkstra(Graph *const graph);
GraphStatus DoBFS(Graph *const graph);
GraphStatus ProgramEnd(Graph *const graph);
GraphStatus DoSpecialSearch(Graph *const graph);
GraphStatus DoGraphviz(Graph *const graph);

#endif
