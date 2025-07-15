#include "enums.h"
#include "templates.h"
#include <iostream>

struct LocalMap { //����� 1 �����
	arr3d<Tile, 64, 64, 256>* tileMap; //����������, ���� �����, ���������� � ���� ��� Z X Y, �������� ��� X Y Z (��������)
	//Tile* tileMap; 3d
	int viewedZLevel;// z ������� ������
	int seed; //��� ����
};

struct GlobalMap {

	arr3d<Tile, 64, 64, 256>* tileMap; //����������, ���� �����, ���������� � ���� ��� Z X Y, �������� ��� X Y Z (��������)
	int viewedZLevel;// z ������� ������
	int seed;
	int* perlinGrid; //1d perlin grid
	int octaveAmount; //amount of perlin octaves
	int initSize; //initial size of grid (in squares, not corners)

	int getOctaveOffset(int octave) {//helper for perlin noise access
		int offset = 0;
		for (int i = 0; i < octave; i++) {
			offset += (int)pow((pow(2, i) * initSize + 1), 2);
		}
		return offset;
	}

	GlobalMap(int seed, int amountOfOctaves, int initGridSIze) :
		seed(seed), octaveAmount(amountOfOctaves), initSize(initGridSIze), viewedZLevel(128) {
	};

};