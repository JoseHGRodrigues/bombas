#include "geo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figure.h"
#include "list.h"

#define MAX_LINE_BUFFER 512

typedef struct {
  char family[64];
  char weight[3];
  int size;
} Ts;

static Ts *tsInit() {
  Ts *t = (Ts *)malloc(sizeof(Ts));
  if (t) {
      strcpy(t->family, "Arial");
      strcpy(t->weight, "n");
      t->size = 12;
  }
  return t;
}

static Figure processCircle(const char *line) {
  int id;
  double x, y, r;
  char colorB[32], colorF[32];
  if (sscanf(line, "c %d %lf %lf %lf %s %s", &id, &x, &y, &r, colorB, colorF) == 6) {
    Figure newFig = figureInit(CIRCLE);
    setCircle(newFig, id, x, y, r, colorB, colorF);
    return newFig;
  }
  return NULL;
}

static Figure processRectangle(const char *line) {
  int id;
  double x, y, w, h;
  char colorB[32], colorF[32];
  if (sscanf(line, "r %d %lf %lf %lf %lf %s %s", &id, &x, &y, &w, &h, colorB, colorF) == 7) {
    Figure newFig = figureInit(RECTANGLE);
    setRectangle(newFig, id, x, y, w, h, colorB, colorF);
    return newFig;
  }
  return NULL;
}

static Figure processLine(const char *line) {
  int id;
  double x1, y1, x2, y2;
  char color[32];
  if (sscanf(line, "l %d %lf %lf %lf %lf %s", &id, &x1, &y1, &x2, &y2, color) == 6) {
    Figure newFig = figureInit(LINE);
    setLine(newFig, id, x1, y1, x2, y2, color);
    return newFig;
  }
  return NULL;
}

static Figure processText(const char *line, Ts *t) {
  int id;
  double x, y;
  char colorB[32], colorF[32], anchor;
  char text[MAX_LINE_BUFFER];
  
  if (sscanf(line, "t %d %lf %lf %s %s %c %[^\n]", &id, &x, &y, colorB, colorF, &anchor, text) == 7) {
    Figure newFig = figureInit(TEXT);
    setText(newFig, id, x, y, colorB, colorF, anchor, text, t->family, t->weight, t->size);
    return newFig;
  }
  return NULL;
}

static void processTs(const char *line, Ts *t) {
  char family[64], weight[3];
  int size;
  if (sscanf(line, "ts %s %s %d", family, weight, &size) == 3) {
      strcpy(t->family, family);
      strcpy(t->weight, weight);
      t->size = size;
  }
}

static void parseGeoLine(char *lineBuffer, List figureList, Ts *t) {
  char command[16];
  Figure newFig = NULL;

  lineBuffer[strcspn(lineBuffer, "\r\n")] = 0;

  if (sscanf(lineBuffer, "%s", command) != 1)
    return;

  if (strcmp(command, "c") == 0)
    newFig = processCircle(lineBuffer);
  else if (strcmp(command, "r") == 0)
    newFig = processRectangle(lineBuffer);
  else if (strcmp(command, "l") == 0)
    newFig = processLine(lineBuffer);
  else if (strcmp(command, "t") == 0)
    newFig = processText(lineBuffer, t);
  else if (strcmp(command, "ts") == 0)
    processTs(lineBuffer, t);

  if (newFig != NULL) {
    listAddLast(figureList, newFig);
  }
}

void processGeoFile(const char *geoFilePath, List figureList) {
  FILE *file = fopen(geoFilePath, "r");
  if (!file) {
    return;
  }

  char lineBuffer[MAX_LINE_BUFFER];
  Ts *t = tsInit();

  while (fgets(lineBuffer, sizeof(lineBuffer), file)) {
    parseGeoLine(lineBuffer, figureList, t);
  }

  free(t);
  fclose(file);
}
