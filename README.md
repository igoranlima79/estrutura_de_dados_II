README.md - Implementação de Árvore Red-Black em C
Descrição
Este projeto contém uma implementação completa de uma Árvore Red-Black em C, que é uma árvore binária de busca balanceada. A estrutura mantém propriedades que garantem um balanceamento aproximado para operações eficientes de inserção, remoção e busca. O código inclui operações fundamentais como rotação à esquerda/direita, inserção com correção das propriedades da árvore, remoção e travessia inorder mostrando as cores dos nós.

Funcionalidades
Criação da árvore com nó sentinela NIL para folhas

Inserção balanceada com recoloração e rotações

Remoção com manutenção do balanceamento por consertos pós-exclusão

Busca de nós por chave

Impressão inorder dos valores com indicação das cores (vermelho ou preto)

Liberação completa da memória alocada​

Estrutura do Código
Tipos e estruturas para nós (RBNode) e árvore (RBTree)

Funções auxiliares para manipulação de nós NIL, criação de nós e inicialização da árvore

Funções para rotações esquerda e direita fundamentais para reestruturação da árvore

Algoritmos para inserção e correção (fixup) das propriedades Red-Black

Algoritmo de remoção que também corrige propriedades da árvore após exclusão

Função de busca e traversais para depuração e visualização

Função principal (main) para demonstrar inserção, remoção e impressão da árvore

Requisitos
Compilador C padrão (gcc, clang, etc.)

Ambiente Unix/Linux ou Windows com suporte a C

Como Compilar e Rodar
bash
gcc -o redblack redblack.c
./redblack
O programa insere um conjunto fixo de chaves, imprime a árvore, remove algumas chaves e imprime o estado final da árvore após cada remoção.

Uso Básico
Inserção: rb_insert(T, chave);

Busca: rb_search(T, T->root, chave);

Remoção: rb_delete(T, nodo);

Impressão: rb_inorder_print(T, T->root);
