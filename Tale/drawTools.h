#pragma once
#include "SDL.h"
#include "enums.h"

void drawToolsInit(SDL_Renderer* rend);
void drawTile(Tile tile, int x, int y);
void drawFrame();