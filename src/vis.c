#include "vis.h"
#include "tree.h"
#include "figure.h"
#include "list.h"
#include "svg.h"
#include "geom.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <float.h>

// --- Configurações ---
#define VIS_INF 1.0e15 
#define VIS_TOLERANCE 0.000001

#ifndef VIS_PI
#define VIS_PI 3.14159265358979323846
#endif

extern void getLineP(Figure f, double *x1, double *y1, double *x2, double *y2);

// --- Estruturas Internas ---

typedef struct {
    double x, y;
} Vertex;

typedef struct {
    Vertex p1, p2;
    int originalId;
    double angleStart; 
    double angleEnd;
} Segment;

#define TYPE_START 0
#define TYPE_END 1

typedef struct {
    double angle;
    int type;
    Segment *seg;
} Event;

static double g_ox, g_oy;
static double g_currentAngle;

// --- Geometria ---

static double getAngle(double x, double y) {
    double a = atan2(y - g_oy, x - g_ox);
    if (a < 0) a += 2 * VIS_PI;
    return a;
}

static double getRaySegDist(Segment *s, double angle) {
    if (!s) return VIS_INF;
    double d = geomRaySegmentIntersect(g_ox, g_oy, angle, 
                                       s->p1.x, s->p1.y, 
                                       s->p2.x, s->p2.y);
    if (d < 0) return VIS_INF;
    return d;
}

// --- Comparadores ---

int visTreeCompare(const void *a, const void *b) {
    Segment *s1 = (Segment *)a;
    Segment *s2 = (Segment *)b;

    if (s1 == s2) return 0;

    double d1 = getRaySegDist(s1, g_currentAngle);
    double d2 = getRaySegDist(s2, g_currentAngle);

    if (fabs(d1 - d2) > 0.001) {
        return (d1 < d2) ? -1 : 1;
    }
    // Desempate por ID para estabilidade
    if (s1->originalId != s2->originalId) {
        return (s1->originalId < s2->originalId) ? -1 : 1;
    }
    // Desempate final por endereço de memória
    return (s1 < s2) ? -1 : 1;
}

int visEventCompare(const void *a, const void *b) {
    Event *e1 = (Event *)a;
    Event *e2 = (Event *)b;

    if (e1->angle < e2->angle - VIS_TOLERANCE) return -1;
    if (e1->angle > e2->angle + VIS_TOLERANCE) return 1;

    // Prioriza END para limpar obstáculos antigos antes de inserir novos
    if (e1->type != e2->type) {
        return (e1->type == TYPE_END) ? -1 : 1;
    }
    return 0;
}

// --- Sorting ---

static void insertionSortEvents(Event *arr, int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        Event key = arr[i];
        int j = i - 1;
        while (j >= left && visEventCompare(&key, &arr[j]) < 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

static void merge(Event *arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    Event *L = malloc(n1 * sizeof(Event));
    Event *R = malloc(n2 * sizeof(Event));
    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (visEventCompare(&L[i], &R[j]) <= 0) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}

static void mergeSortHybrid(Event *arr, int l, int r, int threshold) {
    if (l < r) {
        if ((r - l + 1) <= threshold) insertionSortEvents(arr, l, r);
        else {
            int m = l + (r - l) / 2;
            mergeSortHybrid(arr, l, m, threshold);
            mergeSortHybrid(arr, m + 1, r, threshold);
            merge(arr, l, m, r);
        }
    }
}

// --- Bounding Box Global ---

static void updateBounds(double x, double y, double *minX, double *minY, double *maxX, double *maxY) {
    if (x < *minX) *minX = x;
    if (x > *maxX) *maxX = x;
    if (y < *minY) *minY = y;
    if (y > *maxY) *maxY = y;
}

static void calculateSceneBounds(List figures, double ox, double oy, double *x1, double *y1, double *x2, double *y2) {
    *x1 = ox; *x2 = ox; *y1 = oy; *y2 = oy;
    if (!figures) return;
    int i = 0; void *data;
    while ((data = listGetPos(figures, i++))) {
        Figure f = (Figure)data;
        int shape = getFigureShape(f);
        if (shape == RECTANGLE) {
            double x, y, w, h; getFigureXY(&x, &y, f); getRectangleWH(f, &w, &h);
            updateBounds(x, y, x1, y1, x2, y2); updateBounds(x+w, y+h, x1, y1, x2, y2);
        } else if (shape == CIRCLE) {
            double x, y, r; getFigureXY(&x, &y, f); r = getCircleR(f);
            updateBounds(x-r, y-r, x1, y1, x2, y2); updateBounds(x+r, y+r, x1, y1, x2, y2);
        } else if (shape == LINE) {
            double lx1, ly1, lx2, ly2; getLineP(f, &lx1, &ly1, &lx2, &ly2);
            updateBounds(lx1, ly1, x1, y1, x2, y2); updateBounds(lx2, ly2, x1, y1, x2, y2);
        }
    }
    double margin = 20.0;
    *x1 -= margin; *y1 -= margin; *x2 += margin; *y2 += margin;
}

// --- Gestão de Segmentos ---

static void addSegment(double x1, double y1, double x2, double y2, List segList, int id) {
    double a1 = getAngle(x1, y1);
    double a2 = getAngle(x2, y2);
    double diff = fabs(a1 - a2);

    // Divisão no eixo 0 (raio positivo X)
    if (diff > VIS_PI) { 
        if (fabs(y2 - y1) > VIS_TOLERANCE) {
            double t = (g_oy - y1) / (y2 - y1);
            double ix = x1 + t * (x2 - x1);
            if (ix >= g_ox) {
                // Segmento 1
                Segment *s1 = malloc(sizeof(Segment));
                s1->p1.x = x1; s1->p1.y = y1; s1->p2.x = ix; s1->p2.y = g_oy; s1->originalId = id;
                double ang1 = (a1 > a2) ? a1 : a2;
                s1->angleStart = ang1; s1->angleEnd = 2 * VIS_PI;

                // Segmento 2
                Segment *s2 = malloc(sizeof(Segment));
                s2->p1.x = ix; s2->p1.y = g_oy; s2->p2.x = x2; s2->p2.y = y2; s2->originalId = id;
                double ang2 = (a1 > a2) ? a2 : a1;
                s2->angleStart = 0.0; s2->angleEnd = ang2;

                listAddLast(segList, s1); listAddLast(segList, s2);
                return;
            }
        }
    }
    Segment *s = malloc(sizeof(Segment));
    s->p1.x = x1; s->p1.y = y1; s->p2.x = x2; s->p2.y = y2; s->originalId = id;
    if (a1 < a2) { s->angleStart = a1; s->angleEnd = a2; }
    else { s->angleStart = a2; s->angleEnd = a1; }
    
    listAddLast(segList, s);
}

static void parseFigures(List figures, List segList, double minX, double minY, double maxX, double maxY) {
    // Adiciona o Mundo (Bounding Box)
    // Importante: A ordem dos vértices deve ser consistente
    addSegment(maxX, minY, maxX, maxY, segList, -1); // Direita
    addSegment(maxX, maxY, minX, maxY, segList, -2); // Baixo
    addSegment(minX, maxY, minX, minY, segList, -3); // Esquerda
    addSegment(minX, minY, maxX, minY, segList, -4); // Cima

    if (!figures) return;
    int i = 0; void *data;
    while ((data = listGetPos(figures, i++))) {
        Figure fig = (Figure)data;
        int shape = getFigureShape(fig);
        int id = getFigureId(fig);
        if (shape == RECTANGLE) {
            double x, y, w, h; getFigureXY(&x, &y, fig); getRectangleWH(fig, &w, &h);
            addSegment(x, y, x+w, y, segList, id); addSegment(x+w, y, x+w, y+h, segList, id);
            addSegment(x+w, y+h, x, y+h, segList, id); addSegment(x, y+h, x, y, segList, id);
        } else if (shape == LINE) {
            double x1, y1, x2, y2; getLineP(fig, &x1, &y1, &x2, &y2);
            addSegment(x1, y1, x2, y2, segList, id);
        } else if (shape == CIRCLE) {
            double cx, cy, r; getFigureXY(&cx, &cy, fig); r = getCircleR(fig);
            double x0 = cx-r, y0 = cy-r, dim = 2*r;
            addSegment(x0, y0, x0+dim, y0, segList, id); addSegment(x0+dim, y0, x0+dim, y0+dim, segList, id);
            addSegment(x0+dim, y0+dim, x0, y0+dim, segList, id); addSegment(x0, y0+dim, x0, y0, segList, id);
        }
    }
}

// --- Funções Públicas ---

bool visIsVisible(List figures, double ox, double oy, double tx, double ty) {
    double old_ox = g_ox; double old_oy = g_oy;
    g_ox = ox; g_oy = oy;
    double distToTarget = sqrt(pow(tx - ox, 2) + pow(ty - oy, 2));
    if (distToTarget < VIS_TOLERANCE) { g_ox = old_ox; g_oy = old_oy; return true; }
    
    double angleToTarget = getAngle(tx, ty);
    double minX, minY, maxX, maxY;
    calculateSceneBounds(figures, ox, oy, &minX, &minY, &maxX, &maxY);
    updateBounds(tx, ty, &minX, &minY, &maxX, &maxY);
    
    List segList = listInit();
    parseFigures(figures, segList, minX, minY, maxX, maxY);
    
    bool blocked = false;
    int i = 0; void *data;
    while ((data = listGetPos(segList, i++))) {
        Segment *s = (Segment *)data;
        if (s->originalId < 0) continue; 
        double wallDist = getRaySegDist(s, angleToTarget);
        if (wallDist < distToTarget - 0.1) { blocked = true; break; }
    }
    i = 0; while ((data = listGetPos(segList, i++))) free(data);
    listFree(segList);
    g_ox = old_ox; g_oy = old_oy;
    return !blocked;
}

void visDrawRegion(List figures, double ox, double oy, FILE *svgFile, char sortType, int sortThreshold) {
    g_ox = ox; g_oy = oy; g_currentAngle = 0.0;

    double minX, minY, maxX, maxY;
    calculateSceneBounds(figures, ox, oy, &minX, &minY, &maxX, &maxY);

    List segList = listInit();
    parseFigures(figures, segList, minX, minY, maxX, maxY);

    int numSegs = 0; int k = 0;
    while (listGetPos(segList, k++)) numSegs++;
    if (numSegs == 0) { listFree(segList); return; }

    int numEvents = numSegs * 2;
    Event *events = malloc(sizeof(Event) * numEvents);
    int evIdx = 0; k = 0; Segment *s;
    
    while ((s = (Segment*)listGetPos(segList, k++))) {
        events[evIdx].angle = s->angleStart; 
        events[evIdx].type = TYPE_START; 
        events[evIdx].seg = s; 
        evIdx++;
        
        events[evIdx].angle = s->angleEnd;   
        events[evIdx].type = TYPE_END;   
        events[evIdx].seg = s; 
        evIdx++;
    }

    if (sortType == 'm') mergeSortHybrid(events, 0, evIdx - 1, sortThreshold);
    else qsort(events, evIdx, sizeof(Event), visEventCompare);

    Tree activeSegs = treeInit(visTreeCompare);
    fprintf(svgFile, "<path d=\"M %lf %lf ", g_ox, g_oy);
    double lastX = -9999, lastY = -9999;

    for (int i = 0; i < evIdx; ) {
        Event e = events[i];
        g_currentAngle = e.angle;

        // 1. Desenha Ponto Anterior
        Segment *oldClosest = (Segment *)treeMin(activeSegs);
        if (oldClosest) {
            double dist = getRaySegDist(oldClosest, g_currentAngle);
            if (dist < VIS_INF) {
                double hx = g_ox + cos(g_currentAngle) * dist;
                double hy = g_oy + sin(g_currentAngle) * dist;
                if (fabs(hx - lastX) > 0.01 || fabs(hy - lastY) > 0.01) {
                    fprintf(svgFile, "L %lf %lf ", hx, hy);
                    lastX = hx; lastY = hy;
                }
            }
        }

        // 2. Atualiza Árvore (Batch)
        while (i < evIdx && fabs(events[i].angle - g_currentAngle) < VIS_TOLERANCE) {
            if (events[i].type == TYPE_START) treeInsert(activeSegs, events[i].seg);
            else treeRemove(activeSegs, events[i].seg);
            i++;
        }

        // 3. Desenha Ponto Novo
        Segment *newClosest = (Segment *)treeMin(activeSegs);
        if (newClosest) {
            double dist = getRaySegDist(newClosest, g_currentAngle);
            if (dist < VIS_INF) {
                double hx = g_ox + cos(g_currentAngle) * dist;
                double hy = g_oy + sin(g_currentAngle) * dist;
                if (fabs(hx - lastX) > 0.01 || fabs(hy - lastY) > 0.01) {
                    fprintf(svgFile, "L %lf %lf ", hx, hy);
                    lastX = hx; lastY = hy;
                }
            }
        }
    }

    fprintf(svgFile, "Z\" fill=\"yellow\" opacity=\"0.5\" stroke=\"none\" />\n");

    treeFree(activeSegs, NULL);
    free(events);
    k = 0; while ((s = (Segment*)listGetPos(segList, k++))) free(s);
    listFree(segList);
}
