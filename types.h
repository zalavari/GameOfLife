#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

typedef struct typeconfig {
    int tileSize;
    int tileBorder;
    int tile;
    int ScreenWidth;
    int ScreenHeight;
    int MenuWidth;
    int BgColor;
    bool VisibleBorders;
    bool Toroidal;
} typeconfig;

typedef struct Tabla {
    char ** T;
    int MapX, MapY;
} Tabla;

typedef struct typeCell {
    int bornmin;
    int bornmax;
    int survivemin;
    int survivemax;
    int occupymin;
    int occupymax;
} typeCell;

extern SDL_Surface *screen;
extern typeconfig Config;
extern Tabla Tablaa;
extern int szinkod[10];
extern typeCell cell[9];

#endif // TYPES_H_INCLUDED
