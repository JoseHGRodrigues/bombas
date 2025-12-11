#ifndef VIS_H
#define VIS_H

#include <stdio.h>
#include "list.h"

/**
 * @brief Calcula a região de visibilidade a partir de um ponto e desenha-a no SVG.
 * @param listaFiguras Lista contendo as figuras.
 * @param ox Coordenada X do observador (ponto de visão).
 * @param oy Coordenada Y do observador.
 * @param svgFile Arquivo SVG aberto para escrita onde o polígono será desenhado.
 */
void visDrawRegion(List figures, double ox, double oy, FILE *svgFile, char sortType, int sortThreshold);

/**
  * @brief Verifica se um ponto alvo (tx, ty) é visível a partir da origem (ox, oy).
 * * @param figures Lista de figuras (obstáculos).
 * @param ox, oy Coordenadas da origem (bomba/observador).
 * @param tx, ty Coordenadas do ponto alvo (centro da figura a testar).
 * @return true se o ponto for visível (não bloqueado), false caso contrário.
 */
bool visIsVisible(List figures, double ox, double oy, double tx, double ty);

#endif // VIS_H
