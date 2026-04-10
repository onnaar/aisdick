#ifndef KEY_SPACE_H
#define KEY_SPACE_H

#include "node.h"

typedef size_t KeyType;

typedef struct KeySpace {
    struct KeySpace *link;
    struct Node *node;
    KeyType key;
} KeySpace;

KeySpace *KeySpaceCreate(const InfoType *const info, KeyType key);
KeySpace *KeySpaceCreateRelease(const InfoType *const info, KeyType key, ReleaseType release);
void KeySpaceDownOutput(const KeySpace *const cur_key_space);
void KeySpaceDelete(KeySpace *key_space);

#endif
