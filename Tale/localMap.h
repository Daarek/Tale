#pragma once
#include "templates.h"
#include "enums.h"

struct LocalMap {
	arr3d<Tile, 64, 64, 256>* tileMap;
	int viewedZLevel;
};