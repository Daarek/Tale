#include "enums.h"
#include "templates.h"
#include <iostream>

struct LocalMap { //����� 1 �����
	arr3d<Tile, 64, 64, 256>* tileMap; //����������, ���� �����, ���������� � ���� ��� Z X Y, �������� ��� X Y Z (��������)
	int viewedZLevel;// z ������� ������
	int seed; //��� ����
};