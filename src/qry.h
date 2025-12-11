#ifndef QRY_H
#define QRY_H

#include "list.h"

/**
 * @brief Processa o arquivo de consultas .qry.
 * @param pathQry Caminho completo para o ficheiro .qry de entrada.
 * @param pathOut Caminho (diretoria) onde o ficheiro .svg será salvo.
 * @param figures Lista contendo as figuras (obstáculos) já lidas do .geo.
 */
void processQry(const char *pathQry, const char *pathOut, List figures, char sortType, int sortThreshold, FILE *txtFile);

#endif
