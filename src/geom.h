#ifndef GEOM_H
#define GEOM_H

#include <stdbool.h>

/**
 * @brief Tipo opaco para um Ponto.
 */
typedef void *Point;

/**
 * @brief Tolerância para comparações de float.
 */
#define EPSILON 0.000000001
#define PI 3.14159265358979323846

/**
 * @brief Cria um novo ponto.
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @return Ponteiro para o novo Ponto opaco.
 */
Point geomPointCreate(double x, double y);

/**
 * @brief Liberta a memória de um ponto.
 * @param p O ponto a libertar.
 */
void geomPointFree(Point p);

/**
 * @brief Obtém a coordenada X de um ponto.
 * @param p O ponto.
 * @return Valor de X.
 */
double geomPointGetX(Point p);

/**
 * @brief Obtém a coordenada Y de um ponto.
 * @param p O ponto.
 * @return Valor de Y.
 */
double geomPointGetY(Point p);

/**
 * @brief Atualiza as coordenadas de um ponto.
 * @param p O ponto.
 * @param x Novo X.
 * @param y Novo Y.
 */
void geomPointSet(Point p, double x, double y);

/**
 * @brief Normaliza um ângulo para o intervalo [0, 2*PI).
 * @param angle Ângulo em radianos.
 * @return Ângulo normalizado.
 */
double geomNormalizeAngle(double angle);

/**
 * @brief Calcula o ângulo de um ponto (px, py) em relação a um centro (cx, cy).
 * @return Ângulo em radianos [0, 2*PI).
 */
double geomCalcAngle(double px, double py, double cx, double cy);

/**
 * @brief Calcula a distância euclidiana ao quadrado entre dois pontos.
 * @param x2, y2 Ponto 2.
 * @return Distância ao quadrado.
 */
double geomDistSq(double x1, double y1, double x2, double y2);

/**
 * @brief Calcula a distância euclidiana real.
 */
double geomDist(double x1, double y1, double x2, double y2);

/**
 * @brief Produto Vetorial (Cross Product) para determinar orientação.
 * @param ax, ay Ponto A (origem do vetor 1).
 * @param bx, by Ponto B (fim do vetor 1).
 * @param cx, cy Ponto C (ponto a testar).
 * @return > 0 se C está à esquerda de AB, < 0 se à direita, 0 se colinear.
 */
double geomCrossProduct(double ax, double ay, double bx, double by, double cx, double cy);

/**
 * @brief Calcula a interseção de um Raio com um Segmento.
 * @param ox, oy Origem do raio.
 * @param angle Ângulo do raio (radianos).
 * @param x1, y1 Início do segmento.
 * @param x2, y2 Fim do segmento.
 * @return A distância da origem até a interseção, ou um valor muito grande
 * (MAX_DIST) se não intersetar.
 */
double geomRaySegmentIntersect(double ox, double oy, double angle, 
                               double x1, double y1, double x2, double y2);

#endif // GEOM_H
