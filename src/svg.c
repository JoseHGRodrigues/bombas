#include "svg.h"
#include "figure.h"
#include "list.h"

#include <stdio.h>
#include <string.h>

static void svgDrawCircle(FILE *svgFile, Figure f) {
  char colorB[32], colorF[32];
  double x, y;
  getFigureColors(f, colorB, colorF);
  getFigureXY(&x, &y, f);
  fprintf(svgFile,
          "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" "
          "/>\n",
          x, y, getCircleR(f), colorB, colorF);
}

static void svgDrawRectangle(FILE *svgFile, Figure f) {
  char colorB[32], colorF[32];
  double x, y, w, h;
  getFigureColors(f, colorB, colorF);
  getFigureXY(&x, &y, f);
  getRectangleWH(f, &w, &h);

  fprintf(svgFile,
          "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" "
          "stroke=\"%s\" fill=\"%s\" />\n",
          x, y, w, h, colorB, colorF);
}

static void svgDrawLine(FILE *svgFile, Figure f) {
  char colorB[32], colorF_dummy[32];
  double x1, y1, x2, y2;
  getFigureColors(f, colorB, colorF_dummy);
  getLineP(f, &x1, &y1, &x2, &y2);

  fprintf(svgFile,
          "\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" "
          "stroke=\"%s\" />\n",
          x1, y1, x2, y2, colorB);
}

static void svgDrawText(FILE *svgFile, Figure f) {
  char colorB[32], colorF[32];
  char txt[256], family[64], weight[3];
  char anchor;
  char anchorStr[16];
  char svgWeight[16];
  double x, y;
  int size;

  getFigureColors(f, colorB, colorF);
  getFigureXY(&x, &y, f);
  getTextTXT(f, txt);
  getTextFml(f, family);
  getTextWgt(f, weight);
  size = getTextSize(f);
  anchor = getTextA(f);

  if (anchor == 'i')
    strcpy(anchorStr, "start");
  else if (anchor == 'm')
    strcpy(anchorStr, "middle");
  else if (anchor == 'f')
    strcpy(anchorStr, "end");
  else
    strcpy(anchorStr, "start");

  if (strcmp(weight, "b") == 0)
    strcpy(svgWeight, "bold");
  else if (strcmp(weight, "b+") == 0)
    strcpy(svgWeight, "bolder");
  else if (strcmp(weight, "l") == 0)
    strcpy(svgWeight, "lighter");
  else
    strcpy(svgWeight, "normal");

  fprintf(
      svgFile,
      "\t<text x=\"%lf\" y=\"%lf\" fill=\"%s\" stroke=\"%s\" "
      "text-anchor=\"%s\" "
      "font-family=\"%s\" font-size=\"%d\" font-weight=\"%s\"> %s</text>\n ",
      x, y, colorF, colorB, anchorStr, family, size, svgWeight, txt);
}

void svgInit(FILE *svgFile) {
  if (!svgFile)
    return;
  fprintf(svgFile,
          "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");
}

void svgClose(FILE *svgFile) {
  if (!svgFile)
    return;
  fprintf(svgFile, "</svg>\n");
}

void svgDrawFigure(FILE *svgFile, Figure f) {
  if (!svgFile || !f)
    return;
  int shape = getFigureShape(f);
  switch (shape) {
  case CIRCLE:
    svgDrawCircle(svgFile, f);
    break;
  case RECTANGLE:
    svgDrawRectangle(svgFile, f);
    break;
  case LINE:
    svgDrawLine(svgFile, f);
    break;
  case TEXT:
    svgDrawText(svgFile, f);
    break;
  default:
    break;
  }
}

void svgDrawAll(FILE *svgFile, List figureList) {
  if (!svgFile || !figureList)
    return;

  int i = 0;
  void *data;
  while ((data = listGetPos(figureList, i)) != NULL) {
    Figure f = (Figure)data;
    svgDrawFigure(svgFile, f);
    i++;
  }
}
