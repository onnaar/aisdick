#include <stdio.h>
#include "tree.h"

int main() {
    size_t w = 0;
    Tree *tree = TreeCreate(10, &w);
    w = 3;
    TreeInsert(tree, 3, &w);
    w = 4;
    TreeInsert(tree, 9, &w);
    w = 313;
    TreeInsert(tree, 11, &w);
    w = 999;
    TreeInsert(tree, 1, &w);
    w = 3;
    TreeInsert(tree, 8, &w);
    w = 3;
    TreeInsert(tree, 99, &w);
    w = 3;
    TreeInsert(tree, 5, &w);
    w = 3;
    TreeInsert(tree, 4, &w);
    w = 123;
    TreeInsert(tree, 45, &w);
    w = 731;
    TreeInsert(tree, 14, &w);
    TreeInsert(tree, 2, &w);
    w = 3;
    TreeOutput(tree);
    printf("\n\n");
    TreeOutputFormatted(tree);
    printf("\n\n");
//    TreeOutputFormattedReverse(tree);
    TreeDelete(tree);
    return 0;
}
