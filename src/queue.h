#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Um tipo opaco para uma Queue.
 */
typedef void *Queue;
/**
 * @brief ponteiro void para datas na queue.
 */
typedef void *Data;
/**
 * @brief Aloca e inicializa uma nova queue vazia.
 * @return Um ponteiro Queue para a nova queue, ou NULL se a alocação
 * falhar.
 */
Queue queueInit();

/**
 * @brief Liberta toda a memória associada à fila.
 * @param q A fila a ser libertada.
 */
void queueFree(Queue q);

/**
 * @brief Adiciona um item (ponteiro) ao final da queue.
 * @param q A queue.
 * @param data O ponteiro (void*) para o dado a ser armazenado.
 * @return 1 em caso de sucesso, 0 se a fila for NULL ou falhar a alocação
 */
int queueEnqueue(Queue q, void *data);

/**
 * @brief Remove e retorna o item (ponteiro) do início (front) da queue.
 * @param q A queue.
 * @return O ponteiro (void*) que estava no início, ou NULL se a queue.
 * estiver vazia ou NULL.
 */
void *queueDequeue(Queue q);

/**
 * @brief Verifica se a queue está vazia.
 * @param q A queue.
 * @return true (verdadeiro) se a fila estiver vazia ou for NULL,
 * false (falso) caso contrário.
 */
bool queueIsEmpty(Queue q);

/**
 * @brief Obtém o número atual de elementos na queue.
 * @param q A queue.
 * @return O número de elementos, ou -1 se a fila for NULL.
 */
int queueGetSize(Queue q);

#endif // QUEUE_H
