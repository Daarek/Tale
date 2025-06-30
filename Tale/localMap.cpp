#include "enums.h"
#include "templates.h"

struct LocalMap { //карта 1 чанка
	arr3d<Tile, 64, 64, 256>* tileMap; //собственно, сама карта, Z X Y
	int viewedZLevel;
};