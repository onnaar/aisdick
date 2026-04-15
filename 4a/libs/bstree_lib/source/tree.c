#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "node.h"

Tree *TreeCreate(size_t key, size_t *info) {
    if (!info) {
        return NULL;
    }
    Tree *tree = (Tree *)calloc(1, sizeof(Tree));
    if (!tree) {
        return NULL;
    }
    tree->root = NodeCreate(NULL, key, info);
    if (!tree->root) {
        return NULL;
    }
    return tree;
}

Node *FindKey(Tree *tree, size_t key) {
    if (!tree) {
        return NULL;
    }
    Node *cur = tree->root;
    RelativeIndex index = LEFT;
    while (cur && cur->key != key) {
        index = LEFT;
        if (key >= cur->key) {
            index = RIGHT;
        }
        cur = cur->relatives[index];
    }
    return cur;
}

TreeStatus TreeInsert(Tree *tree, size_t key, size_t *info) {
    if (!tree || !info) {
        return NOT_EXIST;
    }
    Node *cur = tree->root, *prev = NULL;
    while (cur) {
        prev = cur;
        if (key > cur->key) {
            cur = cur->right;
        } else if (key < cur->key) {
            cur = cur->left;
        } else { 
            while (cur && key == cur->key) {
                prev = cur;
                cur = cur->right;
            }
            break;
        }
    }
    Node *temp = NodeCreate(prev, key, info);
    if (!temp) {
        return MEMORY_ERROR;
    }
    if (!tree->root) {
       tree->root = temp;
       return OK;
    }
    if (cur) {
        prev->right = temp;
        temp->right = cur;
        cur->parent = temp;
    } else {
        if (key > prev->key) {
            prev->right = temp;
        } else {
            prev->left = temp;
        }
    }
    return OK;
}

TreeStatus TreeKeyDelete(Tree *tree, size_t key) {
    if (!tree) {
        return NOT_EXIST;
    }
    Node *target = NULL, *cur = tree->root;
    target = FindKey(tree, key);
    if (!target) {
        return NOT_FOUND;
    }

}

/*
 * func delete(t : Node, v : Node):                 // t

 — дерево, v

 — удаляемый элемент
   p = v.parent                                  // предок удаляемого элемента
   if v.left == null and v.right == null         // первый случай: удаляемый элемент - лист
     if p.left == v
       p.left = null
     if p.right == v
       p.right = null
   else if v.left == null or v.right == null     // второй случай: удаляемый элемент имеет одного потомка
       if v.left == null                 
           if p.left == v
             p.left = v.right
           else
             p.right = v.right
           v.right.parent = p 
       else
           if p.left == v
               p.left = v.left
           else
               p.right = v.left
           v.left.parent = p
   else                                          // третий случай: удаляемый элемент имеет двух потомков
     successor = next(v, t)                   
     v.key = successor.key
     if successor.parent.left == successor
       successor.parent.left = successor.right
       if successor.right != null
         successor.right.parent = successor.parent
     else
       successor.parent.right = successor.right
       if successor.right != null
         successor.right.parent = successor.parent
         */
