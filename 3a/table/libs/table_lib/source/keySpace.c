#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "keySpace.h"
#include "stack.h"

KeySpace *KeySpaceCreate(const InfoType *const info, KeyType key) {
    KeySpace *cur_key_space = (KeySpace *)calloc(1, sizeof(KeySpace));
    if (!cur_key_space) {
        return NULL;
    }
    cur_key_space->key = key;
    ReleaseType first_rel = 0;
    cur_key_space->node = NodeCreate(info, first_rel);
    if (!cur_key_space->node) {
        return NULL;
    }
    return cur_key_space;
}

KeySpace *KeySpaceCreateRelease(const InfoType *const info, const KeyType key, const ReleaseType release) {
    KeySpace *cur_key_space = KeySpaceCreate(info, key);
    if (!cur_key_space) {
        return NULL;
    }
    cur_key_space->node->release = release;
    return cur_key_space;
}

void KeySpaceDownOutput(const KeySpace *const cur_key_space) {
    if (!cur_key_space) {
        return;
    }
    Node *cur_node = cur_key_space->node;
    Stack *stack = StackCreate();
    while (cur_node) {
        StackPush(stack, cur_node);
        cur_node = cur_node->next;
    }
    bool first = true;
    while ((cur_node = StackPop(stack)) != NULL) {
        if (first) {
            printf("%-10zu | %-8zu | %-20s\n", cur_key_space->key, cur_node->release, cur_node->info);
            first = false;
        } else {
            printf("%-10s | %-8zu | %-20s\n", "", cur_node->release, cur_node->info);
        }
        NodeDelete(cur_node);
    }
    StackFree(stack);
    return;
}

void KeySpaceDelete(KeySpace *key_space) {
    if (!key_space) {
        return;
    }
    Node *cur = key_space->node;
    while (cur) {
        Node *temp = cur;
        cur = cur->next;
        NodeDelete(temp);
    }
    free(key_space);
}
