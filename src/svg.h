#ifndef SVG_H
#define SVG_H

#include "figure.h"
#include "list.h"
#include <stdio.h>

/**
 * @brief Escreve o cabeçalho SVG em um arquivo.
 * @param svgFile Ponteiro para o arquivo SVG (aberto para escrita).
 */
void svgInit(FILE *svgFile);

/**
 * @brief Escreve o rodapé SVG (tag </svg>) em um arquivo.
 * @param svgFile Ponteiro para o arquivo SVG.
 */
void svgClose(FILE *svgFile);

/**
 * @brief Desenha uma única figura no arquivo SVG.
 * @param svgFile Ponteiro para o arquivo SVG.
 * @param f A figura (Figure) a ser desenhada.
 */
void svgDrawFigure(FILE *svgFile, Figure f);

/**
 * @brief Itera sobre uma Queue de Figure.
 * @param svgFile Ponteiro para o arquivo SVG.
 * @param figureList A List contendo as Figure.
 */
void svgDrawAll(FILE *svgFile, List figureList);

#endif // SVG_H
