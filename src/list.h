#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

/*
 * @brief void pointer para uma lista.
 */
typedef void *List;

/*
 * @vrief um void pointer para um dado
 */
typedef void *Data;

/*
*/
typedef unsigned int Pos;

/*
 * @brief inicializa uma List.
 * return retorna uma List vazia.
 */ 
List listInit();

/*
 * @brief libera o espaço de memoria de uma List.
 * @param uma lista.
 * @return true se conseguir liberar o espaço, false caso o contrario.
*/
bool listFree(List l);

/*
 * @brief insere no Head da List um novo elemento.
 * @param Uma lista.
 * @param o dado a ser inserido.
 * @return true se conseguir inserir, false caso não.
*/
bool listInsertAtHead(List l, Data);

/*
 * @brief insere em uma determinada posição da List.
 * @param a List.
 * @param o dado a ser inserido.
 * @param a posição em que vai ser inserido.
 * @return true se conseguir inserir, false se não.
*/
bool listInsertAtPos(List l, Data d, Pos p);

/*
 * @brief insere um novo elemento na ultima posição da List.
 * @param a List.
 * @param o Dado.
 * @return true se conseguir inserir o elemento, false se não.
*/
bool listInsertAtLast(List l, Data d);

/*
 * @brief remove e insere o dado de um elemento da lista e insere em Data.
 * @param a lista.
 * @param o Dado em que vai ser inserido.
 * return true se conseguiu inserir, false se não.
 */
bool listGetHead(List l, Data d);

/*
 * @brief remove da List e insere no Data o elemento em P.
 * @param a List.
 * @param o Data para receber o elemento.
 * @param a posição do elemento da List.
 * @return true se conseguiu remover e inserir em Data, false caso contrario.
*/
bool listGetPos(List l, Data d, Pos p);

/*
 * @brief remove da List o ultimo elemento e insere no Data.
 * @param a List.
 * @param o Data que vai recebr o elemento.
 * @return true se conseguir inserir e remover o elemento, false caso não.
*/
bool listGetLast(List l, Data d);

#endif // !LIST_H
