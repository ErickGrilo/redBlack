//
//  main.c
//  redBlack
//
//  Created by Christopher Early on 4/4/15.
//  Copyright (c) 2015 Christopher Early. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef enum {red, black} rbColor;

typedef struct node {
    int key;
    struct node *right;
    struct node *left;
    struct node *p;
    rbColor color;
} Node;

typedef struct rbTree {
    Node *root;
    Node *nil;
} rbTree;

Node * newNode(rbTree *T, int key) {
    Node *new = (Node*)malloc(sizeof(Node));
    new->key = key;
    new->left = new->right = new->p = T->nil;
    return new;
}

void treeInit(rbTree *T) {
    T->nil = newNode(T, -1);
    T->nil->color = black;
    T->nil->right = T->nil->left = T->nil;
    T->root = T->nil;
}

void _inOrderWalk(rbTree *T, Node *x) {
    if (x != T->nil) {
        _inOrderWalk(T, x->left);
        printf("%d ", x->key);
        _inOrderWalk(T, x->right);
    }
}

void inOrderWalk(rbTree *T) {
    if(T->root == T->nil) printf("Tree is empty");
    else _inOrderWalk(T, T->root);
    printf("\n");
}

Node * treeMinimum(rbTree *T, Node *x) {
    while (x->left != T->nil) x = x->left;
    return x;
}

Node * treeMaximum(rbTree *T, Node *x) {
    while (x->right != T->nil) x = x->right;
    return x;
}

Node * _treeSearch(rbTree *T, Node *x, int key) {
    if((x == T->nil) || (key == x->key)) {
        return x;
    }
    if(key < x->key) return _treeSearch(T, x->left, key);
    else return _treeSearch(T, x->right, key);
}

Node * treeSearch(rbTree *T, int key) {
    Node *result = _treeSearch(T, T->root, key);
    if(result == T->nil) printf("Cannot find '%d'\n", key);
    return result;
}

void leftRotate(rbTree *T, Node *x) {
    Node *y;
    y = x->right;
    x->right = y->left;
    if(y->left != T->nil) y->left->p = x;
    y->p = x->p;
    if (x->p == T->nil) T->root = y;
    else if(x == x->p->left) x->p->left = y;
    else x->p->right = y;
    y->left = x;
    x->p = y;
}

void rightRotate(rbTree *T, Node *x) {
    Node *y;
    y = x->left;
    x->left = y->right;
    if(y->right != T->nil) y->right->p = x;
    y->p = x->p;
    if (x->p == T->nil) T->root = y;
    else if(x == x->p->right) x->p->right = y;
    else x->p->left = y;
    y->right = x;
    x->p = y;
}

void rbTransplant(rbTree *T, Node *u, Node *v) {
    if(u->p == T->nil) T->root = v;
    else if(u == u->p->left) u->p->left = v;
    else u->p->right = v;
    v->p = u->p;
}

void rbDeleteFixup (rbTree *T, Node *x) {
    while((x != T->root) && (x->color == black)) {
        if(x == x->p->left) {
            Node *w = x->p->right;
            if(w->color == red) {
                w->color = black;
                x->p->color = red;
                leftRotate(T, x->p);
                w = x->p->right;
            }
            if((w->left->color == black) && (w->right->color == black)) {
                w->color = red;
                x = x->p;
            }
            else {
                if(w->right->color == black) {
                    w->left->color = black;
                    w->color = red;
                    rightRotate(T, w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = black;
                w->right->color = black;
                leftRotate(T, x->p);
                x = T->root;
            }
        }
        else {
            Node *w = x->p->left;
            if(w->color == red) {
                w->color = black;
                x->p->color = red;
                rightRotate(T, x->p);
                w = x->p->left;
            }
            if((w->right->color == black) && (w->left->color == black)) {
                w->color = red;
                x = x->p;
            }
            else {
                if(w->left->color == black) {
                    w->right->color = black;
                    w->color = red;
                    leftRotate(T, w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = black;
                w->left->color = black;
                rightRotate(T, x->p);
                x = T->root;
            }
        }
    }
    x->color = black;
}

void rbDelete(rbTree *T, Node *z) {
    if(z == T->nil) return;
    Node *x;
    Node *y = z;
    rbColor yOriginalColor = y->color;
    if(z->left == T->nil) {
        x = z->right;
        rbTransplant(T, z, z->right);
    }
    else if(z->right == T->nil) {
        x = z->left;
        rbTransplant(T, z, z->left);
    }
    else {
        y = treeMinimum(T, z->right);
        yOriginalColor = y->color;
        x = y->right;
        if(y->p == z) {
            x->p = y;
        }
        else {
            rbTransplant(T, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        rbTransplant(T, z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
    }
    if(yOriginalColor == black) rbDeleteFixup(T, x);
    free(z);
}

void rbInsertFixup(rbTree *T, Node *z) {
    while(z->p->color == red) {
        if(z->p == z->p->p->left) {
            Node *y = z->p->p->right;
            if(y->color == red) {
                z->p->color = black;
                y->color = black;
                z->p->p->color = red;
                z = z->p->p;
            }
            else {
                if(z == z->p->right) {
                    z = z->p;
                    leftRotate(T, z);
                }
                z->p->color = black;
                z->p->p->color = red;
                rightRotate(T, z->p->p);
            }
        }
        else {
            Node *y = z->p->p->left;
            if(y->color == red) {
                z->p->color = black;
                y->color = black;
                z->p->p->color = red;
                z = z->p->p;
            }
            else {
                if(z == z->p->left) {
                    z = z->p;
                    rightRotate(T, z);
                }
                z->p->color = black;
                z->p->p->color = red;
                leftRotate(T, z->p->p);
            }
        }
    }
    T->root->color = black;
}

void _rbInsert(rbTree *T, Node *z) {
    Node *y = T->nil;
    Node *x = T->root;
    while(x != T->nil) {
        y = x;
        if (z->key < x->key) x = x->left;
        else x = x->right;
    }
    z->p = y;
    if(y == T->nil) T->root = z;
    else if(z->key < y->key) y->left = z;
    else y->right = z;
    z->left = T->nil;
    z->right = T->nil;
    z->color = red;
    rbInsertFixup(T, z);
}

void rbInsert(rbTree *T, int key) {
    _rbInsert(T, newNode(T, key));
}

void rbDestroy(rbTree *T) {
    while(T->root != T->nil) rbDelete(T, T->root);
}

void displayMenu() {
    printf("\nSelect an Option:\n\n");
    printf("        [1] Insert\n");
    printf("        [2] Delete\n");
    printf("        [3] Delete All\n");
    printf("        [4] Exit\n\n");
}

int menuSelect(rbTree *T) {
    int input;
    scanf("%d", &input);
    switch (input) {
        case 1:
            printf("Enter an integer to insert: ");
            scanf("%d", &input);
            rbInsert(T, input);
            return 0;
        case 2:
            printf("Enter an integer to delete: ");
            scanf("%d", &input);
            rbDelete(T, treeSearch(T, input));
            return 0;
        case 3:
            printf("Deleting all keys...\n");
            rbDestroy(T);
            return 0;
        case 4:
            return 1;
        default:
            printf("Error: Invalid entry\n");
            return 0;
    }
}

int main(int argc, const char * argv[]) {

    rbTree T;
    
    treeInit(&T);
    
    printf("Initializing Red-Black Tree...\nInserting 30 random integers...\n");
    
    srand(23);

    for (int i = 0; i<30; i++) rbInsert(&T, rand() % 100);
    
    int exit = 0;
    
    while(!exit) {
        
        printf("\nHere is the Tree:\n");
        
        inOrderWalk(&T);
    
        displayMenu();
    
        exit = menuSelect(&T);
        
    }
    
    /*
    printf("Max: %d\nMin: %d\n", treeMaximum(&T, T.root)->key, treeMinimum(&T, T.root)->key);
    
    printf("Root: %d\n", T.root->key);
    
    printf("Deleting Min & Max...\n");
    
    rbDelete(&T, treeMaximum(&T, T.root));
    
    rbDelete(&T, treeMinimum(&T, T.root));
    
    printf("In-order Traversal:\n");
    
    inOrderWalk(&T);
    
    printf("Enter an integer to search for & delete: ");
    
    int search;
    
    scanf("%d", &search);
    
    rbDelete(&T, treeSearch(&T, search));
    
    printf("In-order Traversal:\n");
    
    inOrderWalk(&T);
    */
    
    printf("Destroying Tree & Exiting...\n");
    
    rbDestroy(&T);
    
    return 0;
}
