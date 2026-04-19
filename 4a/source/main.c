#include <stdio.h>
#include "tree.h"

int main() {
    size_t w = 0;
    Tree *tree = TreeCreate(1, &w);
    w = 3;
    TreeInsert(tree, 2, &w);
    w = 4;
    TreeInsert(tree, 9, &w);
    w = 313;
    TreeInsert(tree, 11, &w);
    w = 999;
    TreeInsert(tree, 1, &w);
    w = 3;
    TreeInsert(tree, 2, &w);
    w = 123;
    TreeInsert(tree, 45, &w);
    w = 731;
    TreeInsert(tree, 11, &w);
    TreeOutput(tree);
    TreeDelete(tree);
    return 0;
}
