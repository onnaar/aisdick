#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "scapegoat.h"
#include "stack.h"
#include "vector.h"

ScapegoatTree *TreeCreate(const CompareFunc cmp, const DestroyFunc destroy_key_func, const DestroyFunc destroy_value_func) {
    if (!cmp) {
        return NULL;
    }
    ScapegoatTree *tree = (ScapegoatTree *)calloc(1, sizeof(ScapegoatTree));
    if (!tree) {
        return NULL;
    }
    tree->compare = cmp;
    tree->destroy_key = destroy_key_func;
    tree->destroy_value = destroy_value_func;
    return tree;
}

static inline size_t GetMaxHeight(const size_t size) {
    if (size == 0) {
        return 0;
    }
    return (size_t)(log((double)size) / LOG_INV_ALPHA);
}

static inline size_t TreeCountNodes(Node *const root) {
    return TreeTraverse(root, NULL, NULL);
}

TreeStatus TreeInsert(ScapegoatTree *const tree, void *const key, void *const value) {
    if (!tree || !key) {
        return TREE_NOT_VALID;
    }
    Node *current_node = tree->root;
    Node *parent_node = NULL;
    SiblingIndex last_branch_side = LEFT;
    Vector *path_vector = VectorCreate(VECTOR_INITIAL_CAPACITY);
    if (!path_vector) {
        return TREE_MEMORY_ERROR;
    }
    while (current_node) {
        int compare_result = tree->compare(key, current_node->key);
        if (compare_result == 0) {
            VectorFree(path_vector, free);
            return TREE_DUPLICATE;
        }
        TreeStackElement *path_element = (TreeStackElement *)calloc(1, sizeof(TreeStackElement));
        if (!path_element) {
            VectorFree(path_vector, free);
            return TREE_MEMORY_ERROR;
        }
        path_element->node = current_node;
        last_branch_side = (compare_result > 0) ? RIGHT : LEFT;
        path_element->branch_side = last_branch_side;
        if (VectorPush(path_vector, path_element) != VECTOR_OK) {
            free(path_element);
            VectorFree(path_vector, free);
            return TREE_MEMORY_ERROR;
        }
        parent_node = current_node;
        current_node = current_node->relatives[last_branch_side];
    }
    Node *new_node = (Node *)calloc(1, sizeof(Node));
    if (!new_node) {
        VectorFree(path_vector, free);
        return TREE_MEMORY_ERROR;
    }
    new_node->key = key;
    new_node->value = value;
    if (!parent_node) {
        tree->root = new_node;
    } else {
        parent_node->relatives[last_branch_side] = new_node;
    }
    tree->size++;
    if (tree->size > tree->max_size) {
        tree->max_size = tree->size;
    }
    if (path_vector->size > GetMaxHeight(tree->size)) {
        size_t scapegoat_index = TREE_INDEX_NOT_FOUND;
        size_t child_subtree_size = 1;
        for (size_t i = path_vector->size; i > 0; i--) {
            TreeStackElement *current_element = (TreeStackElement *)path_vector->data[i - 1];
            Node *potential_scapegoat = current_element->node;
            SiblingIndex sibling_side = (current_element->branch_side == LEFT) ? RIGHT : LEFT;
            size_t sibling_subtree_size = TreeCountNodes(potential_scapegoat->relatives[sibling_side]);
            size_t parent_subtree_size = 1 + child_subtree_size + sibling_subtree_size;
            if ((double)child_subtree_size > (double)parent_subtree_size * ALPHA) {
                scapegoat_index = i - 1;
                break;
            }
            child_subtree_size = parent_subtree_size;
        }
        if (scapegoat_index != TREE_INDEX_NOT_FOUND) {
            TreeStackElement *scapegoat_element = (TreeStackElement *)path_vector->data[scapegoat_index];
            Node *old_scapegoat_node = scapegoat_element->node;
            size_t old_scapegoat_size = TreeCountNodes(old_scapegoat_node);
            Node *new_balanced_root = TreeRebuild(old_scapegoat_node, old_scapegoat_size);
            if (scapegoat_index == 0) {
                tree->root = new_balanced_root;
            } else {
                TreeStackElement *scapegoat_parent_element = (TreeStackElement *)path_vector->data[scapegoat_index - 1];
                scapegoat_parent_element->node->relatives[scapegoat_parent_element->branch_side] = new_balanced_root;
            }
        }
    }
    VectorFree(path_vector, free);
    return TREE_OK;
}

static Node *BuildBalancedSubtree(Vector *node_vector) {
    if (!node_vector || node_vector->size == 0) {
        return NULL;
    }
    Node *new_root = NULL;
    Vector *task_stack = VectorCreate(VECTOR_INITIAL_CAPACITY);
    if (!task_stack) {
        return NULL;
    }
    BuildTask *first_task = (BuildTask *)calloc(1, sizeof(BuildTask));
    if (!first_task) {
        VectorFree(task_stack, NULL);
        return NULL;
    }
    first_task->start = 0;
    first_task->end = node_vector->size - 1;
    first_task->parent = NULL;
    first_task->child_side = LEFT;
    VectorPush(task_stack, first_task);
    while (task_stack->size > 0) {
        BuildTask *current_task = (BuildTask *)VectorPop(task_stack);
        size_t start_index = current_task->start;
        size_t end_index = current_task->end;
        if (start_index > end_index) {
            free(current_task);
            continue;
        }
        if (end_index == TREE_INDEX_NOT_FOUND) {
            free(current_task);
            continue;
        }
        size_t middle_index = start_index + (end_index - start_index) / 2;
        Node *local_root = (Node *)node_vector->data[middle_index];
        if (!current_task->parent) {
            new_root = local_root;
        } else {
            current_task->parent->relatives[current_task->child_side] = local_root;
        }
        if (middle_index < end_index) {
            BuildTask *right_task = (BuildTask *)calloc(1, sizeof(BuildTask));
            if (right_task) {
                right_task->start = middle_index + 1;
                right_task->end = end_index;
                right_task->parent = local_root;
                right_task->child_side = RIGHT;
                VectorPush(task_stack, right_task);
            }
        }
        if (middle_index > start_index) {
            BuildTask *left_task = (BuildTask *)calloc(1, sizeof(BuildTask));
            if (left_task) {
                left_task->start = start_index;
                left_task->end = middle_index - 1;
                left_task->parent = local_root;
                left_task->child_side = LEFT;
                VectorPush(task_stack, left_task);
            }
        }
        free(current_task);
    }
    VectorFree(task_stack, free);
    return new_root;
}

Node *TreeRebuild(Node *const old_sub_root, const size_t sub_tree_size) {
    if (!old_sub_root || sub_tree_size == 0) {
        return NULL;
    }
    Vector *node_vector = TreeFlatten(old_sub_root, sub_tree_size);
    if (!node_vector) {
        return NULL;
    }
    Node *new_sub_root = BuildBalancedSubtree(node_vector);
    VectorFree(node_vector, NULL);
    return new_sub_root;
}

size_t TreeTraverse(Node *const root, const TraverseActionFunc action, void *context) {
    if (!root) {
        return 0;
    }
    size_t count = 0;
    Stack *stack = StackCreate();
    if (!stack) {
        return 0;
    }
    Node *current_node = root;
    Node *left_node = NULL;
    while (current_node || !IsStackEmpty(stack)) {
        while (current_node) {
            StackPush(stack, current_node);
            current_node = current_node->relatives[RIGHT];
        }
        current_node = (Node *)StackPop(stack);
        left_node = current_node->relatives[LEFT];
        if (action) {
            action(current_node, context);
        }
        count++;
        current_node = left_node;
    }
    StackFree(stack);
    return count;
}

void *TreeFind(const ScapegoatTree *const tree, const void *const key) {
    if (!tree || !key || !tree->root) {
        return NULL;
    }
    Node *current_node = tree->root;
    while (current_node) {
        int compare_result = tree->compare(key, current_node->key);
        if (compare_result == 0) {
            return current_node->value;
        }
        SiblingIndex side = (compare_result > 0) ? RIGHT : LEFT;
        current_node = current_node->relatives[side];
    }
    return NULL;
}

static void Flatten(Node *const node, void *context) {
    FlattenContext *flatten_context = (FlattenContext *)context;
    flatten_context->node_array[flatten_context->current_index] = node;
    node->relatives[LEFT] = NULL;
    node->relatives[RIGHT] = NULL;
    if (flatten_context->current_index > 0) {
        flatten_context->current_index--;
    }
}

Vector *TreeFlatten(Node *const root, const size_t sub_tree_size) {
    if (!root) {
        return NULL;
    }
    Vector *node_vector = VectorCreate(sub_tree_size);
    if (!node_vector) {
        return NULL;
    }
    node_vector->size = sub_tree_size;
    FlattenContext context = {};
    context.node_array = node_vector->data;
    context.current_index = sub_tree_size - 1;
    TreeTraverse(root, Flatten, &context);
    return node_vector;
}

TreeStatus TreeRemove(ScapegoatTree *const tree, const void *const key) {
    if (!tree || !key) {
        return TREE_NOT_VALID;
    }
    Node *current_node = tree->root;
    Node *parent_node = NULL;
    SiblingIndex side_from_parent = LEFT;
    while (current_node) {
        int compare_result = tree->compare(key, current_node->key);
        if (compare_result == 0) {
            break;
        }
        parent_node = current_node;
        side_from_parent = (compare_result > 0) ? RIGHT : LEFT;
        current_node = current_node->relatives[side_from_parent];
    }
    if (!current_node) {
        return TREE_NOT_FOUND;
    }
    if (tree->destroy_key) {
        tree->destroy_key(current_node->key);
    }
    if (tree->destroy_value) {
        tree->destroy_value(current_node->value);
    }
    if (current_node->relatives[LEFT] && current_node->relatives[RIGHT]) {
        Node *successor_node = current_node->relatives[RIGHT];
        Node *successor_parent = current_node;
        SiblingIndex successor_side = RIGHT;
        while (successor_node->relatives[LEFT]) {
            successor_parent = successor_node;
            successor_side = LEFT;
            successor_node = successor_node->relatives[LEFT];
        }
        current_node->key = successor_node->key;
        current_node->value = successor_node->value;
        parent_node = successor_parent;
        side_from_parent = successor_side;
        current_node = successor_node;
    }
    Node *child_node = (current_node->relatives[LEFT]) ? current_node->relatives[LEFT] : current_node->relatives[RIGHT];
    if (!parent_node) {
        tree->root = child_node;
    } else {
        parent_node->relatives[side_from_parent] = child_node;
    }
    free(current_node);
    tree->size--;
    if ((double)tree->size < (double)tree->max_size * ALPHA) {
        tree->root = (tree->size > 0) ? TreeRebuild(tree->root, tree->size) : NULL;
        tree->max_size = tree->size;
    }
    return TREE_OK;
}

static void Free(Node *const node, void *context) {
    Vector *node_vector = (Vector *)context;
    VectorPush(node_vector, node);
}

void TreeFree(ScapegoatTree *tree) {
    if (!tree) {
        return;
    }
    if (tree->root) {
        Vector *node_vector = VectorCreate(VECTOR_INITIAL_CAPACITY);
        if (node_vector) {
            TreeTraverse(tree->root, Free, node_vector);
            for (size_t i = 0; i < node_vector->size; i++) {
                Node *current_node = (Node *)node_vector->data[i];
                if (tree->destroy_key) {
                    tree->destroy_key(current_node->key);
                }
                if (tree->destroy_value) {
                    tree->destroy_value(current_node->value);
                }
                free(current_node);
            }
            VectorFree(node_vector, NULL);
        }
    }
    free(tree);
}
