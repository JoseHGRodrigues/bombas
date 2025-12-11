#ifndef TREE_H
#define TREE_H

#include <stdbool.h>
#include <stdio.h>

/**
 * @brief ponterio void para a Árvore.
 */
typedef void *Tree;

/**
 * @brief Ponteiro de função para comparação de dois elementos.
 * Deve retornar:
 * < 0 se a < b
 * > 0 se a > b
 * 0 se a == b
 */
typedef int (*TreeCmp)(const void *a, const void *b);

/**
 * @brief Ponteiro de função para libertar a memória do dado armazenado.
 */
typedef void (*TreeFreeData)(void *data);

/**
 * @brief Ponteiro de função para processar/imprimir um nó.
 */
typedef void (*TreeProcess)(void *data);

/**
 * @brief Inicializa uma nova árvore vazia.
 * @param cmp Função de comparação que define a ordem dos nós.
 * @return Um ponteiro (Tree) para a nova árvore, ou NULL se falhar.
 */
Tree treeInit(TreeCmp cmp);

/**
 * @brief Liberta toda a memória da árvore.
 * @param t A árvore a ser liberada.
 * @param freeData Função para libertar o conteúdo dos nós (pode ser NULL).
 */
void treeFree(Tree t, TreeFreeData freeData);

/**
 * @brief Insere um novo elemento na árvore.
 * @param t A árvore.
 * @param data O ponteiro para o dado a armazenar.
 * @return true se inseriu com sucesso, false caso contrário.
 */
bool treeInsert(Tree t, void *data);

/**
 * @brief Remove um elemento específico da árvore.
 * A busca pelo elemento a remover é feita usando a função de comparação.
 * @param t A árvore.
 * @param data O ponteiro contendo a chave/dado a ser removido.
 * @return O ponteiro para o dado removido, ou NULL se o elemento não foi encontrado.
 */
void *treeRemove(Tree t, void *data);

/**
 * @brief Busca um elemento na árvore.
 * @param t A árvore.
 * @param data O dado a ser buscado.
 * @return O ponteiro para o dado encontrado, ou NULL se não existir.
 */
void *treeSearch(Tree t, void *data);

/**
 * @brief Retorna o MENOR elemento da árvore.
 * @param t A árvore.
 * @return O ponteiro para o menor dado, ou NULL se a árvore estiver vazia.
 */
void *treeMin(Tree t);

/**
 * @brief Retorna o MAIOR elemento da árvore.
 * @param t A árvore.
 * @return O ponteiro para o maior dado, ou NULL se a árvore estiver vazia.
 */
void *treeMax(Tree t);

/**
 * @brief Verifica se a árvore está vazia.
 * @param t A árvore.
 * @return true se vazia ou se t for NULL, false caso contrário.
 */
bool treeIsEmpty(Tree t);

/**
 * @brief Percorre a árvore em ordem executando a função func.
 * @param t A árvore.
 * @param func Função a aplicar em cada dado.
 */
void treeTraverse(Tree t, TreeProcess func);

/**
 * @brief Retorna a altura da árvore.
 * @param t A árvore.
 * @return A altura (inteiro).
 */
int treeHeight(Tree t);

#endif // TREE_H
