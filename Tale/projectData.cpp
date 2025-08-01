#include "localMap.h"
#include "menus.h"
#include "enums.h"
#include "SDL.h"

struct Data {
	GlobalMap* globalMap;//������ �������� ����
	SDL_Renderer* renderer;
	int windowWidth; //������ � ������ ��������
	int windowHeight;
	int scale; //������� ������ ������ ���������� �� ������
	int side;//������ ������� ����� � ��������
	int startOffsetX; //���������� X � Y ������ ������ �������� ���� ������ �������� ������ �����
	int startOffsetY;
	Screen screen;
	Menus menus;//��� ����.
};