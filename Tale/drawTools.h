#pragma once
#include "SDL.h"
#include "enums.h"
#include "templates.h"

void drawToolsInit(SDL_Renderer* rend, int w, int h);
void drawTile(Tile tile, int x, int y);
void drawFrame(const arr3d<Tile, 64, 64, 256>* arr, int z);
void drawMainMenu();