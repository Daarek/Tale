#include "enums.h"
#include "templates.h"
#include <iostream>
/*
struct LocalMap { //����� 1 �����
	arr3d<Tile, 64, 64, 256>* tileMap; //����������, ���� �����, ���������� � ���� ��� Z X Y, �������� ��� X Y Z (��������)
	//Tile* tileMap; 3d
	int viewedZLevel;// z ������� ������
	int seed; //��� ����
};
*/
struct GlobalMap {

	arr3d<Tile, 64, 64, 256>* tileMap; //����������, ���� �����, ���������� � ���� ��� Z X Y, �������� ��� X Y Z (��������)

	int globalMapSideSize; //how long is the side of global map (in chunks)
	GlobalTile* globalMap; //1d ���������� ����� (�������)

	int viewedZLevel;// z ������� ������
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