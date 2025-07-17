#pragma once
#include "templates.h"
#include "enums.h"
#include <iostream>
/*
struct LocalMap {
	arr3d<Tile, 64, 64, 256>* tileMap;
	//Tile* tileMap;
	int viewedZLevel;
	int seed;
};
*/
struct GlobalMap {
	arr3d<Tile, 64, 64, 256>* tileMap;
	int globalMapSideSize;
	GlobalTile* globalMap;
	int viewedZLevel;
	int seed;
	float* perlinGrid;
	int octaveAmount;
	int initSize;
	int getOctaveOffset(int octave);
};