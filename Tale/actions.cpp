#include "enums.h"
#include "projectData.h"
static Data* data;

void actions_getData(Data* d) {
	data = d;
}

void placeBlock(Tile block, int x, int y, int z) {
	(*data->globalMap->tileMap)[z][x][y] = block;
};