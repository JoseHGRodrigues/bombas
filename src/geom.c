#include "geom.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_DIST_VAL 100000.0

typedef struct {
    double x;
    double y;
} PointImpl;

Point geomPointCreate(double x, double y) {
    PointImpl *p = (PointImpl *)malloc(sizeof(PointImpl));
    if (p) {
        p->x = x;
        p->y = y;
    }
    return (Point)p;
}

void geomPointFree(Point p) {
    if (p) free(p);
}

double geomPointGetX(Point p) {
    if (!p) return 0.0;
    return ((PointImpl *)p)->x;
}

double geomPointGetY(Point p) {
    if (!p) return 0.0;
    return ((PointImpl *)p)->y;
}

void geomPointSet(Point p, double x, double y) {
    if (p) {
        PointImpl *impl = (PointImpl *)p;
        impl->x = x;
        impl->y = y;
    }
}

double geomNormalizeAngle(double angle) {
    while (angle < 0) angle += 2 * PI;
    while (angle >= 2 * PI) angle -= 2 * PI;
    return angle;
}

double geomCalcAngle(double px, double py, double cx, double cy) {
    return geomNormalizeAngle(atan2(py - cy, px - cx));
}

double geomDistSq(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return dx * dx + dy * dy;
}

double geomDist(double x1, double y1, double x2, double y2) {
    return sqrt(geomDistSq(x1, y1, x2, y2));
}

double geomCrossProduct(double ax, double ay, double bx, double by, double cx, double cy) {
    double vec1x = bx - ax;
    double vec1y = by - ay;
    
    double vec2x = cx - ax;
    double vec2y = cy - ay;

    return (vec1x * vec2y) - (vec1y * vec2x);
}

double geomRaySegmentIntersect(double ox, double oy, double angle, 
                               double x1, double y1, double x2, double y2) {
    double dx = cos(angle);
    double dy = sin(angle);

    double v2x = x2 - x1;
    double v2y = y2 - y1;
    
    double v1x = ox - x1;
    double v1y = oy - y1;

    double v3x = -dy; 
    double v3y = dx;
    
    double dot = v2x * v3x + v2y * v3y;
    
    if (fabs(dot) < EPSILON) return MAX_DIST_VAL;
    
    double t1 = (v2x * v1y - v2y * v1x) / dot; 
    double t2 = (v1x * v3x + v1y * v3y) / dot; 

    if (t2 >= -EPSILON && t2 <= 1.0 + EPSILON && t1 >= -EPSILON)
        return t1;

    return MAX_DIST_VAL;
}
