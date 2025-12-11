#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Um tipo opaco para uma Lista Ligada.
 */
typedef void *List;

/**
 * @brief Aloca e inicializa uma nova lista ligada vazia.
 * @return Um ponteiro List para a nova lista, ou NULL se a alocação
 * falhar.
 */
List listInit();

/**
 * @brief Liberta toda a memória associada à lista.
 * @param l A lista a ser libertada.
 */
void listFree(List l);

/**
 * @brief Adiciona um item ao início da lista.
 * @param l A lista.
 * @param data O ponteiro para o dado a ser armazenado.
 * @return true em caso de sucesso, false se a lista for NULL ou falhar a alocação
 */
bool listAddFirst(List l, void *data);

/**
 * @brief Adiciona um item em uma posição da lista.
 * @param l A lista.
 * @param data o ponteiro para o dado ser armazenado.
 * @param pos a posição em que o item esta.
 * @return true se conseguiur pegar o dado, false se a lista for NULL, se pos for maior que o tamanho da lista e caso falhar
 */
bool listAddPos(List l, void *data, int pos);

/**
 * @brief Adiciona um item na ultima posição da lista.
 * @param l A lista.
 * @param data o ponteiro para o dado ser armazenado.
 * @return true em caso de sucesso, false se a lista for NULL ou falhar.
 */
bool listAddLast(List l, void *data);

/**
 * @brief Retorna o item removido do início da lista.
 * @param l A lista.
 * @return O ponteiro (void*) que está no início, ou NULL se a lista
 * estiver vazia ou for NULL.
 */
void *listGetFirst(List l);

/**
 * @brief Retornao item removido de uma posição da lista.
 * @param l A list
 * @param pos A posição a ser removido
 * @return o ponteiro para o item.
 */
void *listGetPos(List l, int pos);

/**
 * @brief Retorna o ultimo elemento da lista
 * @param l A lista.
 * @return o ponteiro do item.
 */
void *listGetLast(List l);

/**
 * @brief Verifica se a lista está vazia.
 * @param list A lista.
 * @return true (verdadeiro) se a lista estiver vazia ou for NULL,
 * false (falso) caso contrário.
 */
bool listIsEmpty(List l);

/**
 * @brief Obtém o número atual de elementos na lista.
 * @param list A lista.
 * @return O número de elementos, ou -1 se a lista for NULL.
 */
int listGetSize(List l);

#endif // LINKED_LIST_H
