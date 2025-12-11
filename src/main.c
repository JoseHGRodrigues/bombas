#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"
#include "geo.h"
#include "qry.h"
#include "svg.h"
#include "figure.h"

typedef struct {
    char *bed;
    char *bsd;
    char *geoName;
    char *qryName;
    
    char *fullGeoPath;
    char *fullQryPath;

    char sortType;
    int inValue;
} Config;

static char *getBaseName(const char *filename) {
    const char *slash = strrchr(filename, '/');
    const char *start = slash ? slash + 1 : filename;
    
    const char *dot = strrchr(start, '.');
    size_t len = dot ? (size_t)(dot - start) : strlen(start);
    
    char *basename = malloc(len + 1);
    if (!basename) {
        exit(1);
    }
    
    strncpy(basename, start, len);
    basename[len] = '\0';
    return basename;
}

char *joinPath(const char *path, const char *fileName) {
    if (!fileName) return NULL;
    if (!path || strlen(path) == 0) return strdup(fileName);

    size_t len = strlen(path) + strlen(fileName) + 2;
    char *fullPath = malloc(len);
    
    strcpy(fullPath, path);
    if (path[strlen(path) - 1] != '/') {
        strcat(fullPath, "/");
    }
    strcat(fullPath, fileName);
    return fullPath;
}

void initConfig(Config *config) {
    memset(config, 0, sizeof(Config));
    config->sortType = 'q';
    config->inValue = 10;
}

void parseArgs(int argc, char *argv[], Config *config) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            config->bed = strdup(argv[++i]);
        } 
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            config->geoName = strdup(argv[++i]);
        } 
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            config->bsd = strdup(argv[++i]);
        } 
        else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            config->qryName = strdup(argv[++i]);
        } 
        else if (strcmp(argv[i], "-to") == 0 && i + 1 < argc) {
            config->sortType = argv[++i][0];
        } 
        else if (strcmp(argv[i], "-in") == 0 && i + 1 < argc) {
            config->inValue = atoi(argv[++i]);
        }
    }

    if (!config->geoName || !config->bsd) {
        exit(1);
    }

    config->fullGeoPath = joinPath(config->bed, config->geoName);
    if (config->qryName) {
        config->fullQryPath = joinPath(config->bed, config->qryName);
    }
}

void freeConfig(Config *config) {
    if (config->bed) free(config->bed);
    if (config->bsd) free(config->bsd);
    if (config->geoName) free(config->geoName);
    if (config->qryName) free(config->qryName);
    if (config->fullGeoPath) free(config->fullGeoPath);
    if (config->fullQryPath) free(config->fullQryPath);
}

int main(int argc, char *argv[]) {
    Config config;
    initConfig(&config);
    parseArgs(argc, argv, &config);

    List figures = listInit();
    if (!figures) {
        freeConfig(&config);
        return 1;
    }

    processGeoFile(config.fullGeoPath, figures);

    char *geoStem = getBaseName(config.geoName);
    char svgName[256];
    sprintf(svgName, "%s.svg", geoStem);
    char *fullSvgPath = joinPath(config.bsd, svgName);

    FILE *fSvg = fopen(fullSvgPath, "w");
    if (fSvg) {
        svgInit(fSvg);
        svgDrawAll(fSvg, figures);
        svgClose(fSvg);
        fclose(fSvg);
    }
    
    free(fullSvgPath);

    if (config.fullQryPath) {
        char *qryStem = getBaseName(config.qryName);
        
        char mergedName[512];
        sprintf(mergedName, "%s-%s.svg", geoStem, qryStem);
        char txt[512];
        sprintf(txt,"%s-%s.txt",geoStem,qryStem);
        
        char *fullTxtPath = joinPath(config.bsd, txt); 
        char *fullQryOutPath = joinPath(config.bsd, mergedName);
        
        FILE *txtFile = fopen(fullTxtPath, "w"); 
        
        if (txtFile) {
            processQry(config.fullQryPath, fullQryOutPath, figures, config.sortType, config.inValue, txtFile);
            fclose(txtFile);
        } else {
            fprintf(stderr, "ERRO: Não foi possível abrir o arquivo de log TXT em: %s\n", fullTxtPath);
            processQry(config.fullQryPath, fullQryOutPath, figures, config.sortType, config.inValue, NULL);
        }
        
        free(qryStem);
        free(fullQryOutPath);
        free(fullTxtPath);
    }

    free(geoStem);

    int i = 0;
    void *fig;
    while((fig = listGetPos(figures, i++))) {
        figureFree(fig);
    }
    listFree(figures);

    freeConfig(&config);
    return 0;
}
