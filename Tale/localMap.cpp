#include "enums.h"
#include "templates.h"
#include <iostream>

struct LocalMap { //карта 1 чанка
	arr3d<Tile, 64, 64, 256>* tileMap; //собственно, сама карта, обращаться к нему как Z X Y, задавать как X Y Z (наверное)
	int viewedZLevel;// z уровень камеры
	int seed; //сид мира
};