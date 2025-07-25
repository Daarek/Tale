#pragma once
#include "SDL.h"
#include "enums.h"
#include "templates.h"
#include "localMap.h"
#include "projectData.h"


void drawTools_getData(Data* d);
void drawTile(Tile tile, int x, int y);
void drawFrame(const arr3d<Tile, 64, 64, 256>* arr, int z);
void drawMainMenu();
void drawMap();