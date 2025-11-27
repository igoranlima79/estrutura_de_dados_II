#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    int key;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct RBTree {
    RBNode *root;
    RBNode *NIL;
} RBTree;

// Criar a sentinela NIL (única por árvore)
static RBNode* rb_create_nil() {
    RBNode *nil = (RBNode*)malloc(sizeof(RBNode));
    if (!nil) { perror("malloc"); exit(EXIT_FAILURE); }
    nil->key = 0;
    nil->color = BLACK;
    nil->left = nil->right = nil->parent = NULL;
    return nil;
}

// Inicializar árvore
RBTree* rb_create() {
    RBTree *T = (RBTree*)malloc(sizeof(RBTree));
    if (!T) { perror("malloc"); exit(EXIT_FAILURE); }
    T->NIL = rb_create_nil();
    // apontar sentinela para si mesmo para facilitar verificações
    T->NIL->left = T->NIL->right = T->NIL->parent = T->NIL;
    T->root = T->NIL;
    return T;
}

RBNode* rb_create_node(RBTree *T, int key) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    if (!node) { perror("malloc"); exit(EXIT_FAILURE); }
    node->key = key;
    node->color = RED; // novo nó inicialmente vermelho
    node->left = node->right = node->parent = T->NIL;
    return node;
}

// Rotação esquerda
void left_rotate(RBTree *T, RBNode *x) {
    RBNode *y = x->right;
    assert(y != T->NIL);

    x->right = y->left;
    if (y->left != T->NIL) y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == T->NIL) T->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;

    y->left = x;
    x->parent = y;
}

// Rotação direita
void right_rotate(RBTree *T, RBNode *y) {
    RBNode *x = y->left;
    assert(x != T->NIL);

    y->left = x->right;
    if (x->right != T->NIL) x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == T->NIL) T->root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;

    x->right = y;
    y->parent = x;
}

// Inserção simples (BST) seguida do conserto das propriedades RB
void rb_insert(RBTree *T, int key) {
    RBNode *z = rb_create_node(T, key);
    RBNode *y = T->NIL;
    RBNode *x = T->root;

    while (x != T->NIL) {
        y = x;
        if (z->key < x->key) x = x->left;
        else x = x->right;
    }

    z->parent = y;
    if (y == T->NIL) T->root = z;
    else if (z->key < y->key) y->left = z;
    else y->right = z;

    z->left = z->right = T->NIL;
    z->color = RED;

    // Fixup
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right; // tio
            if (y->color == RED) {
                // Caso 1
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Caso 2
                    z = z->parent;
                    left_rotate(T, z);
                }
                // Caso 3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                right_rotate(T, z->parent->parent);
            }
        } else {
            // simétrico
            RBNode *y = z->parent->parent->left; // tio
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(T, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                left_rotate(T, z->parent->parent);
            }
        }
        if (z == T->root) break;
    }
    T->root->color = BLACK;
}

// Transplanta subárvore u com v
void rb_transplant(RBTree *T, RBNode *u, RBNode *v) {
    if (u->parent == T->NIL) T->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}

RBNode* tree_minimum(RBTree *T, RBNode *x) {
    while (x->left != T->NIL) x = x->left;
    return x;
}

// Correção após exclusão
void rb_delete_fixup(RBTree *T, RBNode *x) {
    while (x != T->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right; // irmão
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                left_rotate(T, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(T, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                left_rotate(T, x->parent);
                x = T->root;
            }
        } else {
            // simétrico
            RBNode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                right_rotate(T, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(T, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                right_rotate(T, x->parent);
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}

// Remoção
void rb_delete(RBTree *T, RBNode *z) {
    RBNode *y = z;
    RBNode *x;
    Color y_original_color = y->color;

    if (z->left == T->NIL) {
        x = z->right;
        rb_transplant(T, z, z->right);
    } else if (z->right == T->NIL) {
        x = z->left;
        rb_transplant(T, z, z->left);
    } else {
        y = tree_minimum(T, z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rb_transplant(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rb_transplant(T, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    // liberar z
    // NOTA: quem usar pode querer salvar os dados antes de free
    free(z);

    if (y_original_color == BLACK) rb_delete_fixup(T, x);
}

// Buscar por chave
RBNode* rb_search(RBTree *T, RBNode *x, int key) {
    while (x != T->NIL && key != x->key) {
        if (key < x->key) x = x->left;
        else x = x->right;
    }
    return x;
}

// Impressão inorder mostrando cores
void rb_inorder_print(RBTree *T, RBNode *node) {
    if (node == T->NIL) return;
    rb_inorder_print(T, node->left);
    printf("%d(%s) ", node->key, node->color == RED ? "R" : "B");
    rb_inorder_print(T, node->right);
}

// Função utilitária para liberar todos os nós
void rb_free_node(RBTree *T, RBNode *node) {
    if (node == T->NIL) return;
    rb_free_node(T, node->left);
    rb_free_node(T, node->right);
    free(node);
}

void rb_destroy(RBTree *T) {
    if (!T) return;
    rb_free_node(T, T->root);
    free(T->NIL);
    free(T);
}

// Exemplo de uso
int main(void) {
    RBTree *T = rb_create();

    int keys[] = {20, 15, 25, 10, 5, 1, 8, 12, 18, 30, 28, 40};
    size_t n = sizeof(keys)/sizeof(keys[0]);

    printf("Inserindo: ");
    for (size_t i = 0; i < n; ++i) {
        printf("%d ", keys[i]);
        rb_insert(T, keys[i]);
    }
    printf("\nInorder (chave(cor)): ");
    rb_inorder_print(T, T->root);
    printf("\n");

    // Buscar e remover alguns nós
    int to_delete[] = {10, 20, 30};
    size_t m = sizeof(to_delete)/sizeof(to_delete[0]);
    for (size_t i = 0; i < m; ++i) {
        RBNode *z = rb_search(T, T->root, to_delete[i]);
        if (z != T->NIL) {
            printf("Removendo %d...\n", to_delete[i]);
            rb_delete(T, z);
            printf("Inorder: "); rb_inorder_print(T, T->root); printf("\n");
        } else {
            printf("Chave %d nao encontrada.\n", to_delete[i]);
        }
    }

    rb_destroy(T);
    return 0;
}
