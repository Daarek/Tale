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
	int viewedChunkX;
	int viewedChunkY;
	int seed;
	float* perlinGrid;
	int octaveAmount;
	int initSize;
	int getOctaveOffset(int octave) {//helper for perlin noise access
		int offset = 0;
		for (int i = 0; i < octave; i++) {
			offset += (int)pow((pow(2, i) * initSize + 1), 2);
		}
		return offset;
	};
	
};