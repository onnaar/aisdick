#include "vertex.h"
#include <stdlib.h>

Vertex *VertexCreate() {
    return (Vertex *)calloc(1, sizeof(Vertex));
}

void VertexFree(void *const value) {
    if (!value) {
        return;
    }
    Vertex *vertex = (Vertex *)value;
    free(vertex);
}
