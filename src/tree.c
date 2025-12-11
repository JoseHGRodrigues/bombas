#include "tree.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct node {
    void *data;
    struct node *left;
    struct node *right;
    int height;
} Node;

typedef struct {
    Node *root;
    TreeCmp compare;
} TreeStruct;

static int height(Node *n) {
    if (n == NULL) return 0;
    return n->height;
}

static int max(int a, int b) {
    return (a > b) ? a : b;
}

static Node *newNode(void *data) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

static Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

static Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

static int getBalance(Node *n) {
    if (n == NULL) return 0;
    return height(n->left) - height(n->right);
}

Tree treeInit(TreeCmp cmp) {
    TreeStruct *tree = (TreeStruct *)malloc(sizeof(TreeStruct));
    if (tree != NULL) {
        tree->root = NULL;
        tree->compare = cmp;
    }
    return (Tree)tree;
}

static void freeNodeRecursive(Node *n, TreeFreeData freeData) {
    if (n == NULL) return;
    freeNodeRecursive(n->left, freeData);
    freeNodeRecursive(n->right, freeData);
    
    if (freeData) {
        freeData(n->data);
    }
    free(n);
}

void treeFree(Tree t, TreeFreeData freeData) {
    TreeStruct *tree = (TreeStruct *)t;
    if (tree == NULL) return;
    freeNodeRecursive(tree->root, freeData);
    free(tree);
}

static Node *insertRecursive(Node *node, void *data, TreeCmp cmp, bool *success) {
    if (node == NULL) {
        *success = true;
        return newNode(data);
    }

    int comparison = cmp(data, node->data);

    if (comparison < 0)
        node->left = insertRecursive(node->left, data, cmp, success);
    else if (comparison > 0)
        node->right = insertRecursive(node->right, data, cmp, success);
    else {
        // Chaves iguais não permitidas ou ignoradas
        *success = false; 
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Balanceamento
    if (balance > 1 && cmp(data, node->left->data) < 0)
        return rightRotate(node);

    if (balance < -1 && cmp(data, node->right->data) > 0)
        return leftRotate(node);

    if (balance > 1 && cmp(data, node->left->data) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && cmp(data, node->right->data) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

bool treeInsert(Tree t, void *data) {
    TreeStruct *tree = (TreeStruct *)t;
    if (tree == NULL) return false;
    
    bool success = false;
    tree->root = insertRecursive(tree->root, data, tree->compare, &success);
    return success;
}

static Node *minValueNode(Node *node) {
    Node *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

static Node *removeRecursive(Node *root, void *data, TreeCmp cmp, void **removedData) {
    if (root == NULL) return root;

    int comparison = cmp(data, root->data);

    if (comparison < 0)
        root->left = removeRecursive(root->left, data, cmp, removedData);
    else if (comparison > 0)
        root->right = removeRecursive(root->right, data, cmp, removedData);
    else {
        if (removedData) *removedData = root->data;

        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            Node *temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = removeRecursive(root->right, temp->data, cmp, NULL);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void *treeRemove(Tree t, void *data) {
    TreeStruct *tree = (TreeStruct *)t;
    if (tree == NULL || tree->root == NULL) return NULL;

    void *removedData = NULL;
    tree->root = removeRecursive(tree->root, data, tree->compare, &removedData);
    return removedData;
}

static Node *searchRecursive(Node *root, void *data, TreeCmp cmp) {
    if (root == NULL) return NULL;
    int comparison = cmp(data, root->data);
    if (comparison == 0) return root;
    if (comparison < 0) return searchRecursive(root->left, data, cmp);
    return searchRecursive(root->right, data, cmp);
}

void *treeSearch(Tree t, void *data) {
    TreeStruct *tree = (TreeStruct *)t;
    if (tree == NULL) return NULL;
    Node *res = searchRecursive(tree->root, data, tree->compare);
    return res ? res->data : NULL;
}

void *treeMin(Tree t) {
    TreeStruct *tree = (TreeStruct *)t;
    if (tree == NULL || tree->root == NULL) return NULL;
    Node *current = tree->root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current->data;
}

void *treeMax(Tree t) {
    TreeStruct *tree = (TreeStruct *)t;
    if (tree == NULL || tree->root == NULL) return NULL;
    Node *current = tree->root;
    while (current->right != NULL) {
        current = current->right;
    }
    return current->data;
}

bool treeIsEmpty(Tree t) {
    TreeStruct *tree = (TreeStruct *)t;
    if (tree == NULL) return true;
    return tree->root == NULL;
}

int treeHeight(Tree t) {
    TreeStruct *tree = (TreeStruct *)t;
    if (tree == NULL) return 0;
    return height(tree->root);
}

static void traverseRecursive(Node *n, TreeProcess func) {
    if (n == NULL) return;
    traverseRecursive(n->left, func);
    func(n->data);
    traverseRecursive(n->right, func);
}

void treeTraverse(Tree t, TreeProcess func) {
    TreeStruct *tree = (TreeStruct *)t;
    if (tree == NULL) return;
    traverseRecursive(tree->root, func);
}
