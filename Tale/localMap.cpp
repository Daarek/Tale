#include "enums.h"
#include "templates.h"
#include <iostream>
/*
struct LocalMap { //карта 1 чанка
	arr3d<Tile, 64, 64, 256>* tileMap; //собственно, сама карта, обращаться к нему как Z X Y, задавать как X Y Z (наверное)
	//Tile* tileMap; 3d
	int viewedZLevel;// z уровень камеры
	int seed; //сид мира
};
*/
struct GlobalMap {

	arr3d<Tile, 64, 64, 256>* tileMap; //собственно, сама карта, обращаться к нему как Z X Y, задавать как X Y Z (наверное)

	int globalMapSideSize; //how long is the side of global map (in chunks)
	GlobalTile* globalMap; //1d глобальная карта (квадрат)

	int viewedZLevel;// z уровень камеры
	int seed;
	float* perlinGrid; //1d perlin grid
	int octaveAmount; //amount of perlin octaves
	int initSize; //initial size of grid (in squares, not corners)

	int getOctaveOffset(int octave) {//helper for perlin noise access
		int offset = 0;
		for (int i = 0; i < octave; i++) {
			offset += (int)pow((pow(2, i) * initSize + 1), 2);
		}
		return offset;
	}

};