#include "figure.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cLen 8
#define PI 3.14159

typedef struct {
  int id;
  double x, y;
  double radius;
  char colorB[cLen], colorF[cLen];
} Circle;

typedef struct {
  int id;
  double x, y;
  double weight, height;
  char colorB[cLen], colorF[cLen];
} Rectangle;

typedef struct {
  int id;
  double x1, y1;
  double x2, y2;
  char color[cLen];
} Line;

typedef struct {
  int id;
  double x, y;
  char colorB[cLen], colorF[cLen];
  char anchor;
  char txt[64];
  char family[64];
  char weight[3];
  int size;
} Text;

typedef struct {
  void *form;
  int shape;
} figure;

Figure figureInit(int shape) {
  figure *f = malloc(sizeof(figure));
  if (!f)
    return NULL;
  f->shape = shape;
  switch (f->shape) {
  case CIRCLE:
    f->form = malloc(sizeof(Circle));
    break;
  case RECTANGLE:
    f->form = malloc(sizeof(Rectangle));
    break;
  case LINE:
    f->form = malloc(sizeof(Line));
    break;
  case TEXT:
    f->form = malloc(sizeof(Text));
    break;
  default:
    free(f);
    return NULL;
  }
  if (!f->form) {
    free(f);
    return NULL;
  }
  return (Figure)f;
}

void figureFree(Figure f) {
  if (!f)
    return;
  figure *del = (figure *)f;
  if (del->form)
    free(del->form);
  free(del);
}

void setCircle(Figure f, int id, double x, double y, double r,
               const char *colorB, const char *colorF) {
  if (!f || ((figure *)f)->shape != CIRCLE)
    return;
  Circle *c = (Circle *)((figure *)f)->form;
  c->id = id;
  c->x = x;
  c->y = y;
  c->radius = r;
  strcpy(c->colorB, colorB);
  strcpy(c->colorF, colorF);
}

void setRectangle(Figure f, int id, double x, double y, double w, double h,
                  const char *colorB, const char *colorF) {
  if (!f || ((figure *)f)->shape != RECTANGLE)
    return;
  Rectangle *r = (Rectangle *)((figure *)f)->form;
  r->id = id;
  r->x = x;
  r->y = y;
  r->weight = w;
  r->height = h;
  strcpy(r->colorB, colorB);
  strcpy(r->colorF, colorF);
}

void setLine(Figure f, int id, double x1, double y1, double x2, double y2,
             const char *color) {
  if (!f || ((figure *)f)->shape != LINE)
    return;
  Line *l = (Line *)((figure *)f)->form;
  l->id = id;
  l->x1 = x1;
  l->y1 = y1;
  l->x2 = x2;
  l->y2 = y2;
  strcpy(l->color, color);
}

void setText(Figure f, int id, double x, double y, const char *colorB,
             const char *colorF, const char anchor, const char *txt,
             const char *family, const char *weight, int size) {
  if (!f || ((figure *)f)->shape != 4)
    return;
  Text *t = (Text *)((figure *)f)->form;
  t->id = id;
  t->x = x;
  t->y = y;
  t->anchor = anchor;
  strcpy(t->colorB, colorB);
  strcpy(t->colorF, colorF);
  strcpy(t->txt, txt);
  strcpy(t->family, family);
  strcpy(t->weight, weight);
  t->size = size;
}

Figure fClone(Figure f) {
  if (!f)
    return NULL;
  figure *fig = (figure *)f;
  Figure new = figureInit(fig->shape);
  if (!new)
    return NULL;
  switch (fig->shape) {
  case CIRCLE:
    Circle *c = (Circle *)fig->form;
    setCircle(new, c->id, c->x, c->y, c->radius, c->colorB, c->colorF);
    break;
  case RECTANGLE:
    Rectangle *r = (Rectangle *)fig->form;
    setRectangle(new, r->id, r->x, r->y, r->weight, r->height, r->colorB,
                 r->colorF);
    break;
  case LINE:
    Line *l = (Line *)fig->form;
    setLine(new, l->id, l->x1, l->y1, l->x2, l->y2, l->color);
    break;
  case TEXT:
    Text *t = (Text *)fig->form;
    setText(new, t->id, t->x, t->y, t->colorB, t->colorF, t->anchor, t->txt,
            t->family, t->weight, t->size);
    break;
  default:
    return NULL;
  }
  return new;
}

void fMoveTo(Figure f, double x, double y) {
  if (!f)
    return;

  figure *fig = (figure *)f;
  switch (fig->shape) {
  case CIRCLE:
    Circle *c = (Circle *)fig->form;
    c->x = x;
    c->y = y;
    break;
  case RECTANGLE:
    Rectangle *r = (Rectangle *)fig->form;
    r->x = x;
    r->y = y;
    break;
  case LINE:
    Line *l = (Line *)fig->form;
    double dx = x - l->x1;
    double dy = y - l->y1;
    l->x1 = x;
    l->y1 = y;
    l->x2 += dx;
    l->y2 += dy;
    break;
  case TEXT:
    Text *t = (Text *)fig->form;
    t->x = x;
    t->y = y;
    break;
  default:
    return;
  }
}

double figureArea(Figure f) {
  if (!f)
    return -1;
  figure *fig = (figure *)f;
  switch (fig->shape) {
  case CIRCLE:
    Circle *c = (Circle *)fig->form;
    return PI * c->radius * c->radius;
  case RECTANGLE:
    Rectangle *r = (Rectangle *)fig->form;
    return r->height * r->weight;
  case LINE:
    Line *l = (Line *)fig->form;
    double dx = l->x2 - l->x1;
    double dy = l->y2 - l->y1;
    return 2 * sqrt(dx * dx + dy * dy);
  case TEXT:
    Text *t = (Text *)fig->form;
    return 20 * strlen(t->txt);
  }
  return 0;
}

int getComplementaryColor(const char *hexColor, char *comHex) {
  const char *format = (hexColor[0] == '#') ? hexColor + 1 : hexColor;
  if (strlen(format) != 6)
    return 0;
  unsigned int r, g, b;
  if (sscanf(hexColor + 1, "%02x%02x%02x", &r, &g, &b) != 3)
    return 0;
  unsigned int comR, comG, comB;
  comR = 255 - r;
  comG = 255 - g;
  comB = 255 - b;
  sprintf(comHex, "#%02x%02x%02x", comR, comG, comB);
  return 1;
}

void figureInvertColors(Figure f) {
  if (!f)
    return;
  figure *fig = (figure *)f;
  char temp[cLen];
  switch (fig->shape) {
  case CIRCLE:
    Circle *c = (Circle *)fig->form;
    strcpy(temp, c->colorB);
    strcpy(c->colorB, c->colorF);
    strcpy(c->colorF, temp);
    break;
  case RECTANGLE:
    Rectangle *r = (Rectangle *)fig->form;
    strcpy(temp, r->colorB);
    strcpy(r->colorB, r->colorF);
    strcpy(r->colorF, temp);
    break;
  case LINE:
    Line *l = (Line *)fig->form;
    char comHex[cLen];
    if (getComplementaryColor(l->color, comHex))
      strcpy(l->color, comHex);

    break;
  case TEXT:
    Text *t = (Text *)fig->form;
    strcpy(temp, t->colorB);
    strcpy(t->colorB, t->colorF);
    strcpy(t->colorF, temp);
    break;
  }
}

int getFigureId(Figure f) {
  if (!f)
    return -1;
  figure *fig = (figure *)f;
  switch (fig->shape) {
  case CIRCLE:
    Circle *c = (Circle *)fig->form;
    return c->id;
    break;
  case RECTANGLE:
    Rectangle *r = (Rectangle *)fig->form;
    return r->id;
    break;
  case LINE:
    Line *l = (Line *)fig->form;
    return l->id;
    break;
  case TEXT:
    Text *t = (Text *)fig->form;
    return t->id;
    break;
  }
  return 0;
}

void getFigureXY(double *x, double *y, Figure f) {
  if (!f)
    return;
  figure *fig = (figure *)f;
  switch (fig->shape) {
  case CIRCLE:
    Circle *c = (Circle *)fig->form;
    *x = c->x;
    *y = c->y;
    break;
  case RECTANGLE:
    Rectangle *r = (Rectangle *)fig->form;
    *x = r->x;
    *y = r->y;
    break;
  case LINE:
    Line *l = (Line *)fig->form;
    *x = l->x1;
    *y = l->y1;
    break;
  case TEXT:
    Text *t = (Text *)fig->form;
    *x = t->x;
    *y = t->y;
    break;
  default:
    *x = 0;
    *y = 0;
  }
}

void getFigureColors(Figure f, char *colorB, char *colorF) {
  if (!f)
    return;
  figure *fig = (figure *)f;
  switch (fig->shape) {
  case CIRCLE:
    Circle *c = (Circle *)fig->form;
    strcpy(colorB, c->colorB);
    strcpy(colorF, c->colorF);
    break;
  case RECTANGLE:
    Rectangle *r = (Rectangle *)fig->form;
    strcpy(colorB, r->colorB);
    strcpy(colorF, r->colorF);
    break;
  case LINE:
    Line *l = (Line *)fig->form;
    strcpy(colorB, l->color);
    strcpy(colorF, "");
    break;
  case TEXT:
    Text *t = (Text *)fig->form;
    strcpy(colorB, t->colorB);
    strcpy(colorF, t->colorF);
    break;
  default:
    strcpy(colorB, "");
    strcpy(colorF, "");
  }
}

double getCircleR(Figure f) {
  if (!f)
    return 0;
  figure *fig = (figure *)f;
  if (fig->shape != CIRCLE)
    return 0;
  Circle *c = (Circle *)fig->form;
  return c->radius;
}

void getRectangleWH(Figure f, double *w, double *h) {
  if (!f)
    return;
  figure *fig = (figure *)f;
  if (fig->shape != RECTANGLE)
    return;
  Rectangle *r = (Rectangle *)fig->form;
  *w = r->weight;
  *h = r->height;
}

void getLineP(Figure f, double *x1, double *y1, double *x2, double *y2) {
  if (!f)
    return;
  figure *fig = (figure *)f;
  if (fig->shape != LINE)
    return;
  Line *l = (Line *)fig->form;
  *x1 = l->x1;
  *x2 = l->x2;
  *y1 = l->y1;
  *y2 = l->y2;
}

void getTextP(Figure f, double *x1, double *y1, double *x2, double *y2) {
  if (!f)
    return;
  figure *fig = (figure *)f;
  if (fig->shape != 4)
    return;
  char anchor = getTextA(f);
  Text *t = (Text *)fig->form;
  int len = strlen(t->txt);
  *y1 = t->y;
  *y2 = t->y;
  if (anchor == 'i') {
    *x1 = t->x;
    *x2 = t->x + 10 * len;
  } else if (anchor == 'm') {
    *x1 = t->x - 10 * len / 2;
    *x2 = t->x + 10 * len / 2;
  } else {
    *x1 = t->x - 10 * len;
    *x2 = t->x;
  }
}

char getTextA(Figure f) {
  if (!f)
    return '\0';
  figure *fig = (figure *)f;
  if (fig->shape != 4)
    return '\0';
  Text *t = (Text *)fig->form;
  return t->anchor;
}

void getTextTXT(Figure f, char *txt) {
  if (!f)
    return;
  figure *fig = (figure *)f;
  if (fig->shape != 4)
    return;
  Text *t = (Text *)fig->form;
  strcpy(txt, t->txt);
}

void getTextWgt(Figure f, char *wgt) {
  if (!f)
    return;
  figure *fig = (figure *)f;
  if (fig->shape != 4)
    return;
  Text *t = (Text *)fig->form;
  strcpy(wgt, t->weight);
}

void getTextFml(Figure f, char *fml) {
  if (!f)
    return;
  figure *fig = (figure *)f;
  if (fig->shape != 4)
    return;
  Text *t = (Text *)fig->form;
  strcpy(fml, t->family);
}

int getTextSize(Figure f) {
  if (!f)
    return 0;
  figure *fig = (figure *)f;
  if (fig->shape != 4)
    return 0;
  Text *t = (Text *)fig->form;
  return t->size;
}

void putFigureColor(Figure f, const char *colorB, const char *colorF) {
  if (!f)
    return;
  figure *fig = (figure *)f;
  switch (fig->shape) {
  case CIRCLE:
    Circle *c = (Circle *)fig->form;
    strcpy(c->colorB, colorB);
    strcpy(c->colorF, colorF);
    break;
  case RECTANGLE:
    Rectangle *r = (Rectangle *)fig->form;
    strcpy(r->colorB, colorB);
    strcpy(r->colorF, colorF);
    break;
  case LINE:
    Line *l = (Line *)fig->form;
    strcpy(l->color, colorB);
    break;
  case TEXT:
    Text *t = (Text *)fig->form;
    strcpy(t->colorB, colorB);
    strcpy(t->colorF, colorF);
    break;
  }
}

int getFigureShape(Figure f) {
  if (!f)
    return 0;
  figure *fig = (figure *)f;
  return fig->shape;
}

int getFigureType(Figure f) {
  if (!f)
    return -1;
  figure *fig = (figure *)f;
  return fig->shape;
}
