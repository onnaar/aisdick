#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tree.h"

#define START 1
#define STOP 10
#define STEP 100000
#define REPEAT_COUNT 7
#define BATCH_SIZE 10000
#define SP_SEARCH_COUNT 100
#define TRAVERSAL_COUNT 10

double duration(struct timespec start, struct timespec end);

void EmptyAction(Node *cur, void *context);

int main() {
    srand(time(NULL));
    FILE *log_file = fopen("timing_all.txt", "w");
    if (!log_file) return 1;
    fprintf(log_file, "#SIZE insert(10k) delete(10k) search(10k) spsearch(100) traversal(10)\n");
    for (size_t cur = START; cur <= STOP; cur++) {
        Tree *tree = TreeCreate();
        size_t size = cur * STEP;
        size_t *all_keys = (size_t *)calloc(size, sizeof(size_t));
        printf("\n%zu элементов\n", size);
        for (size_t i = 0; i < size; i++) {
            NodeInfo *value = (NodeInfo *)calloc(1, sizeof(NodeInfo));
            size_t key = (size_t)rand();
            value->info = (size_t)rand();
            TreeInsert(tree, key, value);
            all_keys[i] = key;
            free(value);
        }
        double t_ins = 0, t_del = 0, t_sea = 0, t_sp = 0, t_trav = 0;
        size_t batch = BATCH_SIZE;
        for (int i = 0; i < REPEAT_COUNT; i++) {
            size_t *ins_k = (size_t *)calloc(batch, sizeof(size_t));
            NodeInfo **ins_v = (NodeInfo **)calloc(batch, sizeof(NodeInfo *));
            size_t *del_k = (size_t *)calloc(batch, sizeof(size_t));
            size_t *sea_k = (size_t *)calloc(batch, sizeof(size_t));
            struct timespec st = {}, en = {};
            struct timespec *st_p = &st, *en_p = &en;
            for (size_t j = 0; j < batch; j++) {
                ins_k[j] = (size_t)rand();
                ins_v[j] = (NodeInfo *)calloc(1, sizeof(NodeInfo));
                ins_v[j]->info = (size_t)rand();
                del_k[j] = (j < (batch * 2 / 3)) ? ins_k[j] : (size_t)rand();
                sea_k[j] = (j < (batch / 2)) ? all_keys[rand() % size] : (size_t)rand();
            }
            clock_gettime(CLOCK_MONOTONIC, st_p);
            for (size_t j = 0; j < batch; j++) {
                TreeInsert(tree, ins_k[j], ins_v[j]);
            }
            clock_gettime(CLOCK_MONOTONIC, en_p);
            t_ins += duration(st, en);
            clock_gettime(CLOCK_MONOTONIC, st_p);
            for (size_t j = 0; j < batch; j++) {
                TreeKeyDelete(tree, del_k[j]);
            }
            clock_gettime(CLOCK_MONOTONIC, en_p);
            t_del += duration(st, en);
            clock_gettime(CLOCK_MONOTONIC, st_p);
            for (size_t j = 0; j < batch; j++) {
                NodeArray *found = FindKey(tree, sea_k[j]);
                if (found) NodeArrayDelete(found);
            }
            clock_gettime(CLOCK_MONOTONIC, en_p);
            t_sea += duration(st, en);
            clock_gettime(CLOCK_MONOTONIC, st_p);
            for (size_t j = 0; j < SP_SEARCH_COUNT; j++) {
                SpSearchStructure *sp_res = SpecialSearch(tree, (size_t)rand());
                if (sp_res) {
                    if (sp_res->array) NodeArrayDelete(sp_res->array);
                    free(sp_res);
                }
            }
            clock_gettime(CLOCK_MONOTONIC, en_p);
            t_sp += duration(st, en);
            clock_gettime(CLOCK_MONOTONIC, st_p);
            for (size_t j = 0; j < TRAVERSAL_COUNT; j++) {
                TreeTraversing(tree, EmptyAction, NULL);
            }
            clock_gettime(CLOCK_MONOTONIC, en_p);
            t_trav += duration(st, en);
            free(ins_k);
            for (size_t j = 0; j < batch; j++) {
                free(ins_v[j]);
            }
            free(ins_v);
            free(del_k);
            free(sea_k);
        }
        fprintf(log_file, "%zu %.7f %.7f %.7f %.7f %.7f\n", size, t_ins/(double)REPEAT_COUNT, t_del/(double)REPEAT_COUNT, t_sea/(double)REPEAT_COUNT, t_sp/(double)REPEAT_COUNT, t_trav/(double)REPEAT_COUNT);
        free(all_keys);
        TreeTraversing(tree, Delete, NULL);
        free(tree);
    }
    fclose(log_file);
    return 0;
}

double duration(struct timespec start, struct timespec end) {
    return (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;
}

void EmptyAction(Node *cur, void *context) {
    (void)cur;
    (void)context;
}
