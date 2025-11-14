#ifndef FIGURE_H
#define FIGURE_H

// --- Tipos de Figuras ---
#define CIRCLE 1
#define RECTANGLE 2
#define LINE 3
#define TEXT 4

/**
 * @brief Um tipo opaco para uma figure.
 */
typedef void *Figure;

/**
 * @brief Aloca e inicializa uma nova figure.
 * @param shape O tipo da figura (CIRCLE, RECTANGLE, LINE, TEXT).
 * @return Um ponteiro (Figure) para a nova figura, ou NULL se a alocação falhar
 * ou o tipo for inválido.
 */
Figure figureInit(int shape);

/**
 * @brief Liberta toda a memória associada a uma figure.
 * @param f A figure a ser libertada.
 */
void figureFree(Figure f);

/**
 * @brief Configura as propriedades de uma figure do tipo CIRCLE.
 * @param f A figure.
 * @param id ID numérico.
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @param r Raio do círculo.
 * @param colorB Cor da borda (em hexadecimal).
 * @param colorF Cor de preenchimento (em hexadecimal).
 */
void setCircle(Figure f, int id, double x, double y, double r,
               const char *colorB, const char *colorF);

/**
 * @brief Configura as propriedades de uma figure do tipo RECTANGLE.
 * @param f A figure.
 * @param id ID numérico.
 * @param x Coordenada X do canto superior esquerdo.
 * @param y Coordenada Y do canto superior esquerdo.
 * @param w Largura.
 * @param h Altura.
 * @param colorB Cor da borda (em hexadecimal).
 * @param colorF Cor de preenchimento (em hexadecimal).
 */
void setRectangle(Figure f, int id, double x, double y, double w, double h,
                  const char *colorB, const char *colorF);

/**
 * @brief Configura as propriedades de uma figure do tipo LINE.
 * @param f A figure.
 * @param id ID numérico.
 * @param x1 Coordenada X1.
 * @param y1 Coordenada Y1.
 * @param x2 Coordenada X2.
 * @param y2 Coordenada Y2.
 * @param color Cor da linha (em hexadecimal).
 */
void setLine(Figure f, int id, double x1, double y1, double x2, double y2,
             const char *color);

/**
 * @brief Configura as propriedades de uma figura do tipo TEXT.
 * @param f A figure.
 * @param id ID numérico.
 * @param x Coordenada X.
 * @param y Coordenada Y.
 * @param colorB Cor da borda.
 * @param colorF Cor de preenchimento.
 * @param anchor Ponto de âncora do texto ('i' - início, 'm' - meio, 'f' - fim).
 * @param txt O conteúdo do texto.
 */
void setText(Figure f, int id, double x, double y, const char *colorB,
             const char *colorF, const char anchor, const char *txt,
             const char *family, const char *weight, int size);

/**
 * @brief Cria e retorna um clone de uma figure.
 * @param f A figure a ser clonada.
 * @return Uma nova figure que é uma cópia de f, ou NULL se falhar.
 */
Figure fClone(Figure f);

/**
 * @brief Move a figura para uma nova posição (x, y).
 * Para linhas, move o ponto (x1, y1) para (x, y) e translada (x2, y2)
 * @param f A figure.
 * @param x Nova coordenada X.
 * @param y Nova coordenada Y.
 */
void fMoveTo(Figure f, double x, double y);

/**
 * @brief Calcula a area de uma figure.
 * (Linha = 2 * comprimento, Texto = 20 * num_caracteres).
 * @param f A figure.
 * @return A área calculada, ou -1 se a figura for NULL.
 */
double figureArea(Figure f);

/**
 * @brief Inverte as cores da figure.
 * - Círculo, Retângulo, Texto: Troca cor de borda (colorB) com preenchimento
 * (colorF).
 * - Linha: Substitui a cor pela sua cor complementar.
 * @param f A figure.
 */
void figureInvertColors(Figure f);

/**
 * @brief Obtém o ID de uma figure.
 * @param f A figure.
 * @return O ID da figure, ou -1 se a figure for NULL.
 */
int getFigureId(Figure f);

/**
 * @brief Obtém a coordenada de referência (x, y) de uma figure.
 * (Centro para Círculo; Canto superior esquerdo para Retângulo; Ponto inicial
 * para Linha; Ponto de âncora para Texto).
 * @param x Ponteiro para armazenar a coordenada X.
 * @param y Ponteiro para armazenar a coordenada Y.
 * @param f A figure.
 */
void getFigureXY(double *x, double *y, Figure f);

/**
 * @brief Obtém as cores de borda e preenchimento de uma figure.
 * @param f A figure.
 * @param colorB Buffer de destino para a cor da borda.
 * @param colorF Buffer de destino para a cor de preenchimento. Para Linha,
 * colorF será preenchido com '\0'.
 */
void getFigureColors(Figure f, char *colorB, char *colorF);

/**
 * @brief Obtém o raio de um CIRCLE.
 * @param f A figure.
 * @return O raio, ou 0 se f não for um CIRCLE.
 */
double getCircleR(Figure f);

/**
 * @brief Obtém a largura (width) e altura (height) de um RECTANGLE.
 * @param f A figure.
 * @param w Ponteiro para armazenar a largura.
 * @param h Ponteiro para armazenar a altura.
 */
void getRectangleWH(Figure f, double *w, double *h);

/**
 * @brief Obtém os pontos inicial (x1, y1) e final (x2, y2) de uma LINE.
 * @param f A figure.
 * @param x1 Ponteiro para armazenar x1.
 * @param y1 Ponteiro para armazenar y1.
 * @param x2 Ponteiro para armazenar x2.
 * @param y2 Ponteiro para armazenar y2.
 */
void getLineP(Figure f, double *x1, double *y1, double *x2, double *y2);

/**
 * @brief Obtem os pontos inicial (x1, y1) e final (x2, y2) de um TEXT.
 * @param f A figure.
 * @param x1 Ponteiro para armazenar x1;
 * @param y1 Ponteiro para armazenar y1;
 * @param x2 Ponteiro para armazenar x2;
 * @param y2 Ponteiro para armazenar y2;
 */
void getTextP(Figure f, double *x1, double *y1, double *x2, double *y2);

/**
 * @brief Obtém o caractere de âncora de um TEXT.
 * @param f A figure.
 * @return O caractere de âncora ('i', 'm', 'f'), ou '\0' se f não for Texto.
 */
char getTextA(Figure f);

/**
 * @brief Obtém o conteúdo de uma figura de TEXT.
 * @param f A figure.
 * @param txt Buffer de destino para o texto.
 */
void getTextTXT(Figure f, char *txt);

/**
 * @brief Obtem o weight da Figure
 * @param f A figure.
 * @param wgt O weight (n: normal; b: bold; b+: bolder; l: lighter);
 */
void getTextWgt(Figure f, char *wgt);

/**
 * @brief Obtem a family do TEXT.
 * @param f A figure.
 * @param fml Buffer para a family.
 */
void getTextFml(Figure f, char *fml);

/**
 * @brief Obtem o size da fonte do TEXT.
 * @param f A figure.
 * @return um inteiro que é o tamamanho da fonte.
 */
int getTextSize(Figure f);

/**
 * @brief Define as cores de uma figure.
 * @param f A figure.
 * @param colorB Nova cor de borda.
 * @param colorF Nova cor de preenchimento.
 */
void putFigureColor(Figure f, const char *colorB, const char *colorF);

/**
 * @brief Obtém o tipo (shape) de uma figure.
 * @param f A figure.
 * @return O tipo (CIRCLE, RECTANGLE, etc.), ou 0 se a figura for NULL.
 */
int getFigureShape(Figure f);

int getFigureType(Figure f);
#endif
