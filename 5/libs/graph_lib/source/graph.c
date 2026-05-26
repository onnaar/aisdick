#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "graph.h"
#include "hash_table.h"
#include "input.h"
#include "vertex.h"
#include "queue.h"
#include "stack.h"

#define MAGIC_WORD "MAZE_GRAPH"
#define DELIMS " \t\n"
#define VERTEX_PARAMETER "v:"
#define EDGE_PARAMETER "e:"
#define SINGLE_DISTANCE 1
#define INITIAL_CAPACITY 16

Graph *GraphCreate() {
    Graph *const graph = (Graph *)calloc(1, sizeof(Graph));
    if (!graph) {
        return NULL;
    }
    graph->data = TableCreate(GraphHash1, GraphHash2, GraphComparePoints, NULL, NULL);
    if (!graph->data) {
        free(graph);
        return NULL;
    }
    graph->id_table = (Vertex **)calloc(INITIAL_CAPACITY, sizeof(Vertex *));
    if (!graph->id_table) {
        TableFree(graph->data);
        free(graph);
        return NULL;
    }
    graph->vertex_counter = 0;
    graph->capacity = INITIAL_CAPACITY;
    return graph;
}

size_t GraphHash1(const void *const key, const size_t capacity) {
    const Point *const point = (const Point *const)key;
    size_t hash = FNV_OFFSET;
    hash ^= point->x;
    hash *= FNV_PRIME;
    hash ^= point->y;
    hash *= FNV_PRIME;
    return hash % capacity;
}

size_t GraphHash2(const void *const key, const size_t capacity) {
    (void)capacity;
    const Point *const point = (const Point *const)key;
    size_t hash_x = point->x * KNUTH_PRIME;
    size_t hash_y = point->y * KNUTH_PRIME;
    size_t hash = hash_x ^ (hash_y);
    return (hash | 1);
}

int GraphComparePoints(const void *const key1, const void *const key2) {
    const Point *const p1 = (const Point *const)key1;
    const Point *const p2 = (const Point *const)key2;
    return !(p1->x == p2->x && p1->y == p2->y);
}

Vertex *GraphFindVertex(const Graph *const graph, const size_t target_id) {
    if (!graph || !graph->id_table || target_id >= graph->vertex_counter) {
        return NULL;
    }
    return graph->id_table[target_id];
}

GraphStatus GraphAddVertex(Graph *const graph, const Point coords, const VertexType type) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    if (TableFind(graph->data, &coords) != NULL) {
        return GRAPH_DUPLICATE;
    }
    if (graph->vertex_counter >= graph->capacity) {
        size_t new_cap = graph->capacity * 2;
        Vertex **new_id_table = (Vertex **)realloc(graph->id_table, new_cap * sizeof(Vertex *));
        if (!new_id_table) {
            return GRAPH_MEMORY_ERROR;
        }
        memset(new_id_table + graph->capacity, 0, (new_cap - graph->capacity) * sizeof(Vertex *));
        graph->id_table = new_id_table;
        graph->capacity = new_cap;
    }
    Vertex *vertex = VertexCreate();
    if (!vertex) {
        return GRAPH_MEMORY_ERROR;
    }
    vertex->coords = coords;
    vertex->type = type;
    vertex->id = graph->vertex_counter;
    TableStatus status = TableInsert(graph->data, &(vertex->coords), vertex);
    if (status != TABLE_OK) {
        free(vertex);
        return GRAPH_MEMORY_ERROR;
    }
    graph->id_table[vertex->id] = vertex;
    graph->vertex_counter++;
    return GRAPH_OK;
}

GraphStatus GraphAddEdge(Graph *const graph, const size_t id, const Neighbours direction) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    Vertex *src = graph->id_table[id];
    if (!src) {
        return GRAPH_NOT_FOUND;
    }
    if (src->adjacency[direction]) {
        return GRAPH_DUPLICATE; 
    }
    if ((direction == LEFT && src->coords.x == 0) || (direction == DOWN && src->coords.y == 0)) {
        return GRAPH_NOT_VALID;
    }
    Point dest_coords = src->coords;
    dest_coords.x += (direction == RIGHT) - (direction == LEFT);
    dest_coords.y += (direction == UP) - (direction == DOWN);
    Vertex *dest = (Vertex *)TableFind(graph->data, &dest_coords);
    if (!dest) {
        return GRAPH_NOT_FOUND;
    }
    src->adjacency[direction] = dest;
    return GRAPH_OK;
}

GraphStatus GraphRemoveEdge(Graph *const graph, const size_t id, const Neighbours direction) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    Vertex *src = graph->id_table[id];
    if (!src) {
        return GRAPH_NOT_FOUND;
    }
    if (!src->adjacency[direction]) {
        return GRAPH_NOT_FOUND;
    }
    src->adjacency[direction] = NULL;
    return GRAPH_OK;
}

GraphStatus GraphRemoveVertex(Graph *const graph, const size_t id) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    Vertex *target = GraphFindVertex(graph, id);
    if (!target) {
        return GRAPH_NOT_FOUND;
    }
    for (Neighbours i = 0; i < 4; i++) {
        if ((target->coords.y == 0 && i == DOWN) || (target->coords.x == 0 && i == LEFT)) {
            continue;
        }
        Point n_coords = target->coords;
        n_coords.x += (i == RIGHT) - (i == LEFT);
        n_coords.y += (i == UP) - (i == DOWN);
        Vertex *neighbour = TableFind(graph->data, &n_coords);
        if (neighbour) {
            for (int j = 0; j < 4; j++) {
                if (neighbour->adjacency[j] == target) {
                    neighbour->adjacency[j] = NULL;
                }
            }
        }
    }
    size_t remove_id = target->id;
    TableStatus status = TableRemove(graph->data, &target->coords);
    if (status == TABLE_OK) {
        VertexFree(target);
        graph->id_table[remove_id] = NULL;
        return GRAPH_OK;
    }
    return GRAPH_NOT_FOUND;
}

GraphStatus GraphUpdateVertex(Graph *const graph, const size_t target_id, const Point new_coords, const VertexType new_type) {
    if (!graph || !graph->id_table || target_id >= graph->vertex_counter) {
        return GRAPH_NOT_VALID;
    }
    Vertex *v = graph->id_table[target_id];
    if (!v) {
        return GRAPH_NOT_FOUND;
    }
    if (v->coords.x == new_coords.x && v->coords.y == new_coords.y) {
        v->type = new_type;
        return GRAPH_OK;
    }
    if (TableFind(graph->data, &new_coords) != NULL) {
        return GRAPH_NOT_VALID;
    }
    Vertex *old_out[4] = {};
    Vertex *old_in[4] = {};
    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {-1, 0, 1, 0};
    //redo
    for (Neighbours i = 0; i < 4; i++) {
        old_out[i] = v->adjacency[i];
        if ((i == DOWN && v->coords.y == 0) || (i == LEFT && v->coords.x == 0)) {
            v->adjacency[i] = NULL;
            continue;
        }
        Point n_coords = v->coords;
        n_coords.x += dx[i];
        n_coords.y += dy[i];
        Vertex *neighbour = TableFind(graph->data, &n_coords);
        if (neighbour) {
            Neighbours opposite = (i + 2) % 4;
            if (neighbour->adjacency[opposite] == v) {
                old_in[i] = neighbour;
                neighbour->adjacency[opposite] = NULL;
            }
        }
        v->adjacency[i] = NULL;
    }
    TableRemove(graph->data, &v->coords);
    v->coords = new_coords;
    TableInsert(graph->data, &v->coords, v);
    for (Neighbours i = 0; i < 4; i++) {
        if ((i == DOWN && v->coords.y == 0) || (i == LEFT && v->coords.x == 0)) {
            continue;
        }
        Point n_coords = v->coords;
        n_coords.x += dx[i];
        n_coords.y += dy[i];
        Vertex *new_neighbour = TableFind(graph->data, &n_coords);
        if (!new_neighbour) {
            continue;
        }
        for (size_t j = 0; j < 4; j++) {
            if (old_out[j] == new_neighbour) {
                v->adjacency[i] = new_neighbour;
            }
            if (old_in[j] == new_neighbour) {
                new_neighbour->adjacency[(i + 2) % 4] = v;
            }
        }
    }
    v->type = new_type;
    return GRAPH_OK;
}

GraphStatus GraphImport(Graph *const graph, const char *const filename) {
    if (!graph || !filename) {
        return GRAPH_NOT_VALID;
    }
    FILE *file = fopen(filename, "r");
    if (!file) {
        return GRAPH_NOT_FOUND;
    }
    const char *dir_names[] = {"UP", "RIGHT", "DOWN", "LEFT"};
    const char *type_names[] = {"EXIT", "TRANSITION", "ENTRANCE"};
    char *magic = my_readline(file);
    if (!magic || (strcmp(magic, MAGIC_WORD)) != 0) {
        if (magic) {
            free(magic);
        }
        fclose(file);
        return GRAPH_WRONG_FORMAT;
    }
    free(magic);
    char *buffer = NULL;
    size_t line_number = 1;
    while((buffer = my_readline(file))) {
        line_number++;
        char *word = my_strtok(buffer, DELIMS);
        if (!word) {
            free(buffer);
            continue;
        }
        if (strcmp(word, EDGE_PARAMETER) != 0 && strcmp(word, VERTEX_PARAMETER) != 0) {
            free(buffer);
            printf("line %zu: WRONG FORMAT\n", line_number);
            continue;
        }
        char *token_x = my_strtok(NULL, DELIMS);
        char *token_y = my_strtok(NULL, DELIMS);
        char *token_last = my_strtok(NULL, DELIMS);
        size_t x = 0, y = 0;
        GraphStatus status = GRAPH_OK;
        if (!token_x || !token_y || !token_last || (StrToZu(token_x, &x) != INPUT_OK) || (StrToZu(token_y, &y) != INPUT_OK)) {
            printf("line %zu: WRONG FORMAT\n", line_number);
            free(buffer);
            continue;
        }
        Point coords = {x, y};
        if (word[0] == 'v') {
            for (VertexType type = 0; type < 3; type++) {
                if (strcmp(type_names[type], token_last) == 0) {
                    status = GraphAddVertex(graph, coords, type - 1);
                    if (status == GRAPH_DUPLICATE) {
                        printf("line %zu: duplicate hasn't inserted\n", line_number);
                    }
                }
            }
        } else {
            Vertex *src_vertex = (Vertex *)TableFind(graph->data, &coords);
            if (!src_vertex) {
                printf("line %zu: source vertex for edge not found\n", line_number);
                free(buffer);
                continue;
            }
            for (Neighbours dir = 0; dir < 4; dir++) {
                if (strcmp(dir_names[dir], token_last) == 0) {
                    status = GraphAddEdge(graph, src_vertex->id, dir);
                    if (status == GRAPH_DUPLICATE) {
                        printf("line %zu: duplicate hasn't inserted\n", line_number);
                    }
                }
            }
        }
        free(buffer);
    }
    fclose(file);
    return GRAPH_OK;
}

GraphStatus GraphExport(const Graph *const graph, const char *const filename) {
    if (!graph || !filename) {
        return GRAPH_NOT_VALID;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        return GRAPH_NOT_VALID;
    }
    const char *dir_names[] = {"UP", "RIGHT", "DOWN", "LEFT"};
    const char *type_names[] = {"EXIT", "TRANSITION", "ENTRANCE"};
    fprintf(file, "%s\n", MAGIC_WORD);
    for (size_t i = 0; i < graph->vertex_counter; i++) {
        if (graph->id_table[i]) {
            Vertex *cur_vertex = graph->id_table[i];
            fprintf(file, "%s %zu %zu %s\n", VERTEX_PARAMETER, cur_vertex->coords.x, cur_vertex->coords.y, type_names[cur_vertex->type + 1]);
        }
    }
    for (size_t i = 0; i < graph->vertex_counter; i++) {
        if (graph->id_table[i]) {
            Vertex *cur_vertex = graph->id_table[i];
            for (int dir = 0; dir < 4; dir++) {
                if (cur_vertex->adjacency[dir]) {
                    fprintf(file, "%s %zu %zu %s\n", EDGE_PARAMETER, cur_vertex->coords.x, cur_vertex->coords.y, dir_names[dir]);
                }
            }
        }
    }
    fclose(file);
    return GRAPH_OK;
}

GraphStatus GraphAdjacencyOutput(const Graph *const graph) {
    if (!graph) {
        return GRAPH_NOT_VALID;
    }
    if (graph->vertex_counter == 0) {
        return GRAPH_EMPTY;
    }
    const char *dir_names[] = {"UP", "RIGHT", "DOWN", "LEFT"};
    const char *vertex_type[] = {"EXIT", "TRANSITION", "ENTRANCE"};
    for (size_t i = 0; i < graph->vertex_counter; i++) {
        if (graph->id_table[i]) {
            Vertex *cur_vertex = graph->id_table[i];
            printf("[%zu %s] -> ", cur_vertex->id, vertex_type[cur_vertex->type + 1]);
            for (Neighbours j = 0; j < 4; j++) {
                if (cur_vertex->adjacency[j]) {
                    printf("[%s: %zu %s] -> ", dir_names[j], cur_vertex->adjacency[j]->id, vertex_type[cur_vertex->adjacency[j]->type + 1]);
                }
            }
            printf("NULL\n");
        }
    }
    return GRAPH_OK;
}

Vertex **ShortestPathDijkstra(const Graph *const graph, const size_t start_id, const size_t finish_id) {
    if (!graph || start_id >= graph->vertex_counter || finish_id >= graph->vertex_counter) {
        return NULL;
    }
    Vertex *start = graph->id_table[start_id];
    Vertex *finish = graph->id_table[finish_id];
    if (!start || !finish || start->type != ENTRANCE || finish->type != EXIT) {
        return NULL;
    }
    size_t number = graph->vertex_counter;
    Vertex **parent = (Vertex **)calloc(number, sizeof(Vertex *));
    if (!parent) {
        return NULL;
    }
    bool *visited = (bool *)calloc(number, sizeof(bool));
    if (!visited) {
        free(parent);
        return NULL;
    }
    int *dist = (int *)calloc(number, sizeof(int));
    if (!dist) {
        free(visited);
        free(parent);
        return NULL;
    }
    for (size_t i = 0; i < number; i++) {
        dist[i] = INT_MAX;
    }
    dist[start->id] = 0;
    for (size_t step = 0; step < number; step++) {
        int min_distance = INT_MAX;
        Vertex *u = NULL;
        for (size_t i = 0; i < number; i++) {
            if (graph->id_table[i] && !visited[i] && dist[i] < min_distance) {
                u = graph->id_table[i];
                min_distance = dist[i];
            }
        }
        if (!u || dist[u->id] == INT_MAX || finish->id == u->id) {
            break;
        }
        visited[u->id] = true;
        for (Neighbours dir = 0; dir < 4; dir++) {
            Vertex *neighbour = u->adjacency[dir];
            if (neighbour && !visited[neighbour->id] && dist[u->id] + 1 < dist[neighbour->id]) {
                dist[neighbour->id] = dist[u->id] + SINGLE_DISTANCE;
                parent[neighbour->id] = u;
            }
        }
    }
    Vertex **path = NULL;
    if (dist[finish->id] == INT_MAX) {
        goto exit;
    }
    int path_len = dist[finish->id] + 1;
    path = (Vertex **)calloc(path_len + 1, sizeof(Vertex *));
    if (path) {
        Vertex *cur = finish;
        size_t i = path_len;
        while (cur) {
            path[--i] = cur;
            cur = parent[cur->id];
        }
        path[path_len] = NULL;
    }
exit:
    free(visited);
    free(dist);
    free(parent);
    return path;
}

Vertex **BFS(const Graph *const graph, const size_t start_id) {
    if (!graph || start_id >= graph->vertex_counter) {
        return NULL;
    }
    Vertex *start = graph->id_table[start_id];
    if (!start || start->type != ENTRANCE) {
        return NULL;
    }
    size_t number = graph->vertex_counter;
    bool *visited = (bool *)calloc(number, sizeof(bool));
    if (!visited) {
        return NULL;
    }
    Vertex **parent = (Vertex **)calloc(number, sizeof(Vertex *));
    if (!parent) {
        free(visited);
        return NULL;
    }
    Vertex **path = NULL;
    Vertex *finish = NULL;
    Queue *queue = QueueCreate();
    if (!queue) {
        goto exit;
    }
    QueueStatus status = QueuePush(queue, start);
    visited[start->id] = true;
    if (status != QUEUE_OK) {
        goto exit;
    }
    while (!IsQueueEmpty(queue)) {
        Vertex *cur_node = (Vertex *)QueuePop(queue);
        if (cur_node->type == EXIT) {
            finish = cur_node;
            break;
        }
        for (Neighbours dir = 0; dir < 4; dir++) {
            Vertex *neighbour = cur_node->adjacency[dir];
            if (neighbour && !visited[neighbour->id]) {
                parent[neighbour->id] = cur_node;
                visited[neighbour->id] = true;
                status = QueuePush(queue, neighbour);
                if (status != QUEUE_OK) {
                    goto exit;
                }
            }
        }
    }
    if (!finish) {
        goto exit;
    }
    Stack *stack = StackCreate();
    if (!stack) {
        goto exit;
    }
    Vertex *cur = finish;
    while (cur) {
        if (StackPush(stack, cur) != STACK_OK) {
            StackFree(stack);
            goto exit;
        }
        cur = parent[cur->id];
    }
    size_t i = 0;
    path = (Vertex **)calloc(number + 1, sizeof(Vertex *));
    if (!path) {
        StackFree(stack);
        goto exit;
    }
    while (!IsStackEmpty(stack)) {
        path[i++] = StackPop(stack); 
    } 
    StackFree(stack);
exit:
    free(visited);
    free(parent);
    QueueFree(queue);
    return path;
}

GraphStatus GraphMakeMST(Graph *const graph) {
    if (!graph || graph->vertex_counter == 0) {
        return GRAPH_NOT_VALID;
    }
    size_t number = graph->vertex_counter;
    GraphStatus status = GRAPH_OK;
    bool *in_tree = (bool *)calloc(number, sizeof(bool));
    Vertex **mst_edges_src = (Vertex **)calloc(number * 4, sizeof(Vertex *));
    Vertex **mst_edges_dst = (Vertex **)calloc(number * 4, sizeof(Vertex *));
    Neighbours *mst_dirs = (Neighbours *)calloc(number * 4, sizeof(Neighbours));
    Queue *queue = QueueCreate();
    if (!in_tree || !mst_edges_src || !mst_edges_dst || !mst_dirs || !queue) {
        status = GRAPH_MEMORY_ERROR;
        goto exit;
    }
    size_t mst_edge_count = 0;
    for (size_t i = 0; i < number; i++) {
        if (graph->id_table[i] && graph->id_table[i]->type == ENTRANCE) {
            QueuePush(queue, graph->id_table[i]);
            in_tree[graph->id_table[i]->id] = true;
        }
    }
    while (!IsQueueEmpty(queue)) {
        Vertex *cur = (Vertex *)QueuePop(queue);
        for (Neighbours dir = 0; dir < 4; dir++) {
            Vertex *neighbour = cur->adjacency[dir];
            if (!neighbour) {
                continue;
            }
            if (!in_tree[neighbour->id]) {
                in_tree[neighbour->id] = true;
                mst_edges_src[mst_edge_count] = cur;
                mst_edges_dst[mst_edge_count] = neighbour;
                mst_dirs[mst_edge_count] = dir;
                mst_edge_count++;
                QueuePush(queue, neighbour);
            } else {
                if (neighbour->type == ENTRANCE) {
                    bool already_exists = false;
                    for (size_t j = 0; j < mst_edge_count; j++) {
                        if (mst_edges_src[j] == cur && mst_edges_dst[j] == neighbour) {
                            already_exists = true;
                            break;
                        }
                    }
                    if (!already_exists) {
                        mst_edges_src[mst_edge_count] = cur;
                        mst_edges_dst[mst_edge_count] = neighbour;
                        mst_dirs[mst_edge_count] = dir;
                        mst_edge_count++;
                    }
                }
            }
        }
    }
    const char *dir_names[] = {"UP", "RIGHT", "DOWN", "LEFT"};
    for (size_t i = 0; i < number; i++) {
        if (!graph->id_table[i]) {
            continue;
        }
        Vertex *cur = graph->id_table[i];
        for (Neighbours dir = 0; dir < 4; dir++) {
            Vertex *neighbour = cur->adjacency[dir];
            if (!neighbour) {
                continue;
            }
            bool keep_edge = false;
            for (size_t j = 0; j < mst_edge_count; j++) {
                if (mst_edges_src[j] == cur && mst_edges_dst[j] == neighbour && mst_dirs[j] == dir) {
                    keep_edge = true;
                    break;
                }
            }
            if (!keep_edge) {
                printf("removed edge from %zu to %zu (direction %s)\n", cur->id, neighbour->id, dir_names[dir]);
                cur->adjacency[dir] = NULL;
            }
        }
    }
exit:
    free(in_tree);
    free(mst_edges_src);
    free(mst_edges_dst);
    free(mst_dirs);
    QueueFree(queue);
    return status;
}

GraphStatus GraphExportDot(const Graph *const graph, const char *const filename, Vertex **const path) {
    if (!graph || !filename) {
        return GRAPH_NOT_VALID;
    }
    if (!graph->vertex_counter) {
        return GRAPH_EMPTY;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        return GRAPH_NOT_FOUND;
    }
    size_t number = graph->vertex_counter;
    fprintf(file, "digraph Maze {\n");
    fprintf(file, "    layout=neato;\n");
    fprintf(file, "    node [shape=box, style=filled, fontname=\"Arial\", width=0.7, height=0.4, fontsize=11, penwidth=1.5];\n");
    fprintf(file, "    edge [fontsize=9, fontname=\"Arial\"];\n");
    for (size_t i = 0; i < number; i++) {
        if (!graph->id_table[i]) {
            continue;
        }
        Vertex *cur = graph->id_table[i];
        const char *color = "lightgray";
        if (cur->type == ENTRANCE) {
            color = "limegreen";
        } else if (cur->type == EXIT) {
            color = "crimson";
        }
        const char *node_style = "";
        if (path) {
            for (size_t j = 0; path[j] != NULL; j++) {
                if (path[j] == cur) {
                    if (cur->type == ENTRANCE || cur->type == EXIT) {
                        node_style = ", color=darkorange, penwidth=3.0";
                    } else {
                        color = "darkorange";
                    }
                    break;
                }
            }
        }
        double visual_x = (double)cur->coords.x * 1.6;
        double visual_y = (double)cur->coords.y * 1.6;
        fprintf(file, "    %zu [label=\"%zu(%zu,%zu)\", fillcolor=%s%s, pos=\"%f,%f!\"];\n", cur->id, cur->id, cur->coords.x, cur->coords.y, color, node_style, visual_x, visual_y);
    }
    const char *dir_names[] = {"UP", "RIGHT", "DOWN", "LEFT"};
    for (size_t i = 0; i < number; i++) {
        if (!graph->id_table[i]) {
            continue;
        }
        Vertex *cur = graph->id_table[i];
        for (Neighbours dir = 0; dir < 4; dir++) {
            Vertex *neighbour = cur->adjacency[dir];
            if (!neighbour) {
                continue;
            }
            const char *edge_style = "";
            if (path) {
                for (size_t j = 0; path[j] != NULL && path[j + 1] != NULL; j++) {
                    if (path[j] == cur && path[j + 1] == neighbour) {
                        edge_style = ", color=darkorange, penwidth=2.5";
                        break;
                    }
                }
            }
            fprintf(file, "    %zu -> %zu [label=\"%s\"%s];\n", cur->id, neighbour->id, dir_names[dir], edge_style);
        }
    }
    fprintf(file, "}\n");
    fclose(file);
    return GRAPH_OK;
}

void GraphFree(Graph *const graph) {
    if (!graph) {
        return;
    }
    for (size_t i = 0; i < graph->vertex_counter; i++) {
        if (graph->id_table[i]) {
            VertexFree(graph->id_table[i]);
        }
    }
    TableFree(graph->data);
    free(graph->id_table);
    free(graph);
}
