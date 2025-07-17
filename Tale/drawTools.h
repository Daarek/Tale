#pragma once
#include "SDL.h"
#include "enums.h"
#include "templates.h"
#include "localMap.h"

void drawToolsInit(SDL_Renderer* rend, GlobalMap* map, int w, int h);
void drawTile(Tile tile, int x, int y);
void drawFrame(const arr3d<Tile, 64, 64, 256>* arr, int z);
void drawMainMenu();
void drawMap();