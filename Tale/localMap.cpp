#include "enums.h"
#include "templates.h"

struct LocalMap { //����� 1 �����
	arr3d<Tile, 64, 64, 256>* tileMap; //����������, ���� �����, Z X Y
	int viewedZLevel;
};