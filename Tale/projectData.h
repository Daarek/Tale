#pragma once
#include "localMap.h"
#include "enums.h"
#include "SDL.h"

struct Data {
	GlobalMap* globalMap;//������ �������� ����
	SDL_Renderer* renderer;
	int windowWidth; //������ � ������ ��������
	int windowHeight;
	int scale; //������� ������ ������ ���������� �� ������
	int startOffsetX; //���������� X � Y ������ ������ �������� ���� ������ �������� ������ �����
	int startOffsetY;
	Screen screen;
};