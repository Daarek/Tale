#pragma once

#include "enums.h"
#include "templates.h"
#include "SDL.h"

arr3d<Tile, 64, 64, 256>* generateRandom();

arr3d<Tile, 64, 64, 256>* generateOnHeightMap(int seed);