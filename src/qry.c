#include "qry.h"
#include "vis.h"
#include "svg.h"
#include "figure.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

// Definindo POLYGON como 5, um valor comum em figure.h para este tipo de projeto.
#ifndef POLYGON
#define POLYGON 5
#endif

// --- Funções Auxiliares ---

static void makeSvgPathWithSuffix(const char *basePath, const char *sfx, char *dest) {
    strcpy(dest, basePath);
    char *dot = strrchr(dest, '.');
    if (dot) *dot = '\0';
    strcat(dest, "-");
    strcat(dest, sfx);
    strcat(dest, ".svg");
}

static const char* getShapeName(int shape) {
    switch (shape) {
        case CIRCLE: return "CIRCULO";
        case RECTANGLE: return "RETANGULO";
        case LINE: return "LINHA";
        case TEXT: return "TEXTO";
        case POLYGON: return "POLIGONO"; 
        default: return "DESCONHECIDO";
    }
}

static void getFigureCenter(Figure f, double *x, double *y) {
    getFigureXY(x, y, f);
    if (getFigureShape(f) == RECTANGLE) {
        double w, h;
        getRectangleWH(f, &w, &h);
        *x += w / 2.0;
        *y += h / 2.0;
    }
}

// --- Comandos ---

static void processA(const char *params, List figures, FILE *txtFile) {
    int idStart, idEnd;
    char orient; 
    
    if (sscanf(params, "%d %d %c", &idStart, &idEnd, &orient) != 3) return;

    List newLines = listInit();
    int i = 0;
    void *data;
    static int g_segIdCounter = 50000;
    
    while ((data = listGetPos(figures, i++))) {
        Figure f = (Figure)data;
        int figId = getFigureId(f);

        if (figId >= idStart && figId <= idEnd) {
            int shape = getFigureShape(f);

            if (shape == CIRCLE) {
                double x, y, r;
                char cb[32], cf[32];
                getFigureXY(&x, &y, f);
                r = getCircleR(f);
                getFigureColors(f, cb, cf);

                double x1, y1, x2, y2;
                if (orient == 'h') {
                    x1 = x - r; y1 = y;
                    x2 = x + r; y2 = y;
                } else {
                    x1 = x; y1 = y - r;
                    x2 = x; y2 = y + r;
                }

                int newId = g_segIdCounter++;
                Figure seg = figureInit(LINE);
                setLine(seg, newId, x1, y1, x2, y2, cb);
                listAddLast(newLines, seg);

                if (txtFile) {
                    // LOG A: reportar id e tipo da figura original, id e extremos dos segmentos produzidos
                    fprintf(txtFile, "A: Figura Original ID %d (%s) -> Segmento Novo ID %d. Extremos: (%.1f, %.1f) e (%.1f, %.1f). Cor: %s\n",
                            figId, getShapeName(shape), newId, x1, y1, x2, y2, cb);
                }
                
                // "Destroi" o círculo original APÓS pegar as informações para o log
                setCircle(f, figId, x, y, 0.0, "none", "none");
            }
        }
    }

    i = 0;
    while ((data = listGetPos(newLines, i++))) {
        listAddLast(figures, data);
    }
    listFree(newLines);
}

static void processD(const char *params, List figures, FILE *mainSvg, const char *baseOutPath, char sortType, int sortThreshold, FILE *txtFile) {
    double x, y;
    char sfx[64];
    
    if (sscanf(params, "%lf %lf %s", &x, &y, sfx) < 3) return;

    FILE *targetSvg = mainSvg;
    bool isSeparateFile = (strcmp(sfx, "-") != 0);
    char auxSvgPath[512];

    if (isSeparateFile) {
        makeSvgPathWithSuffix(baseOutPath, sfx, auxSvgPath);
        targetSvg = fopen(auxSvgPath, "w");
        if (!targetSvg) return;
        svgInit(targetSvg);
        svgDrawAll(targetSvg, figures);
    }

    fprintf(targetSvg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"5\" fill=\"red\" stroke=\"black\" stroke-width=\"2\" />\n", x, y);
    // Chamada para desenhar o polígono (região de visibilidade)
    visDrawRegion(figures, x, y, targetSvg, sortType, sortThreshold); 

    int i = 0;
    void *data;
    while ((data = listGetPos(figures, i++))) {
        Figure f = (Figure)data;
        double fx, fy;
        getFigureCenter(f, &fx, &fy);

        if (getFigureShape(f) == CIRCLE && getCircleR(f) < 0.001) continue;

        // NOVO FILTRO: Pular se a figura é um Anteparo (assumindo LINE)
        if (getFigureShape(f) == LINE) continue; 

        // Checar se o centro da figura está dentro do polígono de visibilidade
        if (visIsVisible(figures, x, y, fx, fy)) {
            int id = getFigureId(f);
            int shape = getFigureShape(f);
            
            if (txtFile) {
                // LOG D: reportar id e tipo das formas destruídas
                fprintf(txtFile, "D: Figura destruída ID=%d Tipo=%s\n", id, getShapeName(shape));
            }
            // Destroi a figura
            if (shape == CIRCLE) setCircle(f, id, fx, fy, 0.0, "none", "none");
            else putFigureColor(f, "none", "none"); 
        }
    }

    if (isSeparateFile) {
        svgClose(targetSvg);
        fclose(targetSvg);
    }
}

static void processP(const char *params, List figures, FILE *mainSvg, const char *baseOutPath, char sortType, int sortThreshold, FILE *txtFile) {
    double x, y;
    char color[32];
    char sfx[64];
    
    int read = sscanf(params, "%lf %lf %s %s", &x, &y, color, sfx);
    if (read < 3) return; 
    if (read == 3) strcpy(sfx, "-");

    FILE *targetSvg = mainSvg;
    bool isSeparateFile = (strcmp(sfx, "-") != 0);
    char auxSvgPath[512];

    if (isSeparateFile) {
        makeSvgPathWithSuffix(baseOutPath, sfx, auxSvgPath);
        targetSvg = fopen(auxSvgPath, "w");
        if (!targetSvg) return;
        svgInit(targetSvg);
        svgDrawAll(targetSvg, figures);
    }

    fprintf(targetSvg, "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"5\" fill=\"%s\" stroke=\"black\" opacity=\"1\" />\n", x, y, color);
    // Chamada para desenhar o polígono (região de visibilidade)
    visDrawRegion(figures, x, y, targetSvg, sortType, sortThreshold);

    int i = 0;
    void *data;
    while ((data = listGetPos(figures, i++))) {
        Figure f = (Figure)data;
        double fx, fy;
        getFigureCenter(f, &fx, &fy);

        if (getFigureShape(f) == CIRCLE && getCircleR(f) < 0.001) continue;

        // NOVO FILTRO: Pular se a figura é um Anteparo (assumindo LINE)
        if (getFigureShape(f) == LINE) continue; 

        // Checar se o centro da figura está dentro do polígono de visibilidade
        if (visIsVisible(figures, x, y, fx, fy)) {
            int id = getFigureId(f);
            int shape = getFigureShape(f);
            
            // Pintar a figura
            putFigureColor(f, color, color);
            
            if (txtFile) {
                // LOG P: reportar id e tipo das formas pintadas
                fprintf(txtFile, "P: Figura pintada ID=%d Tipo=%s Cor=%s\n", id, getShapeName(shape), color);
            }
        }
    }

    if (isSeparateFile) {
        svgClose(targetSvg);
        fclose(targetSvg);
    }
}

static void processCln(const char *params, List figures, FILE *mainSvg, const char *baseOutPath, char sortType, int sortThreshold, FILE *txtFile) {
    double x, y, dx, dy;
    char sfx[64];
    
    int read = sscanf(params, "%lf %lf %lf %lf %s", &x, &y, &dx, &dy, sfx);
    if (read < 4) return;
    if (read == 4) strcpy(sfx, "-");

    static int idCounter = 90000;

    FILE *targetSvg = mainSvg;
    bool isSeparateFile = (strcmp(sfx, "-") != 0);
    char auxSvgPath[512];

    if (isSeparateFile) {
        makeSvgPathWithSuffix(baseOutPath, sfx, auxSvgPath);
        targetSvg = fopen(auxSvgPath, "w");
        if (!targetSvg) return;
        svgInit(targetSvg);
        svgDrawAll(targetSvg, figures);
    }

    fprintf(targetSvg, "\t<text x=\"%lf\" y=\"%lf\" fill=\"blue\" font-weight=\"bold\">CLN</text>\n", x, y);

    List clones = listInit();
    int i = 0;
    void *data;
    
    while ((data = listGetPos(figures, i++))) {
        Figure f = (Figure)data;
        double fx, fy;
        getFigureCenter(f, &fx, &fy);
        
        if (getFigureShape(f) == CIRCLE && getCircleR(f) < 0.001) continue;

        // NOVO FILTRO: Pular se a figura é um Anteparo (assumindo LINE)
        if (getFigureShape(f) == LINE) continue; 

        // Checar se o centro da figura está dentro do polígono de visibilidade
        if (visIsVisible(figures, x, y, fx, fy)) {
            int shape = getFigureShape(f);
            int originalId = getFigureId(f);
            
            double origX, origY;
            char cb[32], cf[32];
            getFigureXY(&origX, &origY, f);
            getFigureColors(f, cb, cf);
            
            Figure nf = figureInit(shape);
            int newId = idCounter++;

            // Lógica de clonagem com deslocamento
            if (shape == CIRCLE) {
                double r = getCircleR(f);
                setCircle(nf, newId, origX + dx, origY + dy, r, cb, cf);
            } 
            else if (shape == RECTANGLE) {
                double w, h;
                getRectangleWH(f, &w, &h);
                setRectangle(nf, newId, origX + dx, origY + dy, w, h, cb, cf);
            }
            else if (shape == LINE) { // Este bloco é alcançado apenas se o filtro for alterado. Por segurança, o deixamos aqui.
                double x1, y1, x2, y2;
                getLineP(f, &x1, &y1, &x2, &y2);
                setLine(nf, newId, x1 + dx, y1 + dy, x2 + dx, y2 + dy, cb);
            }
            
            // Adiciona o clone à lista temporária
            listAddLast(clones, nf);

            if (txtFile) {
                // LOG CLN: id e tipo das figuras originais e dos clones
                fprintf(txtFile, "CLN: Original[ID=%d, Tipo=%s] -> Clone[ID=%d] Deslocado(%.1f, %.1f)\n", 
                        originalId, getShapeName(shape), newId, dx, dy);
            }
        }
    }

    i = 0;
    // Adiciona os clones à lista principal de figuras
    while ((data = listGetPos(clones, i++))) {
        listAddLast(figures, data);
    }
    listFree(clones); 

    if (isSeparateFile) {
        svgClose(targetSvg);
        fclose(targetSvg);
    }
}

static void processQryLine(const char *line, FILE *mainSvg, const char *baseOutPath, FILE *txtFile, List figures, char sortType, int sortThreshold) {
    char command[32];
    char params[512];
    
    memset(command, 0, sizeof(command));
    memset(params, 0, sizeof(params));

    if (sscanf(line, "%s %[^\n]", command, params) < 1) return;

    if (strcmp(command, "a") == 0) 
        processA(params, figures, txtFile);
    else if (strcmp(command, "d") == 0) 
        processD(params, figures, mainSvg, baseOutPath, sortType, sortThreshold, txtFile);
    else if (strcmp(command, "p") == 0) 
        processP(params, figures, mainSvg, baseOutPath, sortType, sortThreshold, txtFile);
    else if (strcmp(command, "cln") == 0) 
        processCln(params, figures, mainSvg, baseOutPath, sortType, sortThreshold, txtFile);
}

void processQry(const char *pathQry, const char *pathOut, List figures, char sortType, int sortThreshold, FILE *txtFile) {
    FILE *fQry = fopen(pathQry, "r");
    if (!fQry) return;

    FILE *fSvg = fopen(pathOut, "w");
    if (!fSvg) {
        fclose(fQry);
        return;
    }

    svgInit(fSvg);
    svgDrawAll(fSvg, figures);

    char line[512];
    while (fgets(line, sizeof(line), fQry)) {
        line[strcspn(line, "\r\n")] = 0;
        processQryLine(line, fSvg, pathOut, txtFile, figures, sortType, sortThreshold);
    }

    svgClose(fSvg);
    fclose(fSvg);
    fclose(fQry);
}
