#include <stdlib.h>
#include "printItem.h"

PrintStackItem *PrintStackItemCreate() {
    return (PrintStackItem *)calloc(1, sizeof(PrintStackItem));
}

void PrintStackItemDelete(PrintStackItem *cur) {
    if (!cur) {
        return;
    }
    free (cur);
}

