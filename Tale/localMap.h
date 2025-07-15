#pragma once
#include "templates.h"
#include "enums.h"
#include <iostream>

struct LocalMap {
	arr3d<Tile, 64, 64, 256>* tileMap;
	//Tile* tileMap;
	int viewedZLevel;
	int seed;
};

struct GlobalMap {
	arr3d<Tile, 64, 64, 256>* tileMap;
	int viewedZLevel;
	int seed;
	int* perlinGrid;
	int octaveAmount;
	int initSize;

	int getOctaveOffset(int octave);
	GlobalMap(int seed, int amountOfOctaves, int initGridSIze);
};