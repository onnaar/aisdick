#include <stdlib.h>
#include "keySpace.h"

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
