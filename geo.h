#ifndef GEO_H
#define GEO_H

#include "queue.h"

/**
 * @brief Processa um arquivo .geo, lendo as figuras e as adicionando
 * a uma lista.
 * @param geoFilePath O caminho completo para o arquivo .geo a ser lido
 * @param figureList A Queue onde as figures criadas serão armazenadas.
 */
void processGeoFile(const char *geoFilePath, Queue figureList);

#endif // GEO_H
