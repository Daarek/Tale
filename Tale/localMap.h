#pragma once
#include "templates.h"
#include "enums.h"
#include <iostream>

struct LocalMap {
	arr3d<Tile, 64, 64, 256>* tileMap;
	int viewedZLevel;
	int seed;
};