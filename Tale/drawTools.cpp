#include "SDL.h"
#include "enums.h"
#include "templates.h"
#include "localMap.h"
#include <iostream>

static SDL_Renderer* renderer = NULL;//renderer
static GlobalMap* globalMap;

static SDL_Texture* placeholder = NULL;//�������� �����������
static SDL_Texture* betaTileset = NULL;//�������� �������     ===> �������� ��� �� 32�32 ����� ������
static SDL_Texture* rat16 = NULL; //���
static SDL_Texture* fog = NULL;//������

static int scale = 64; //� ������� ��� ������� ���������������, ������� ������ ���������� �� �����
static int screenWidth;
static int screenHeight;

static SDL_FRect cut(int x, int y) { //�������, ������� ������ ��������� �������� �� ��� ����������� �� ������
	return { (float)(32 * x), (float)(32 * y), 32.f, 32.f };
};

void drawToolsInit(SDL_Renderer* rend, GlobalMap* map, int w, int h) { //�������, ����� �������� �� ������ �� main ����
	renderer = rend;
	globalMap = map;
	placeholder = IMG_LoadTexture(renderer, "Assets/coconut.png"); //������������� ��������
	betaTileset = IMG_LoadTexture(renderer, "Assets/ts_simple32.png");
	rat16 = IMG_LoadTexture(renderer, "Assets/rat.png");
	fog = IMG_LoadTexture(renderer, "Assets/ts_fog.png");
	screenWidth = w;
	screenHeight = h;
};

static void drawTile(Tile tile, int x, int y) { //���������� ����
	SDL_FRect i; //����� ���� ��������
	int side = (int)(1024 / scale); //������ ������� ����� � ��������
	SDL_FRect pos = { x*side, y*side, side, side }; //�����, ���� �� ������ ���������� ����
	switch (tile) { //�������� ���� ��� ������� � ������� ��� � ��������
	case AIR:
		break;
	case GRASS:
		i = cut(0, 0);
		break;
	case STONE: //�������� �����, �� �����
		i = cut(1, 0);
		break;

	};
	SDL_RenderTexture(renderer, betaTileset, &i, &pos); //������
};

static void drawFog(int lvl, int x, int y) {
	SDL_FRect i = cut(lvl-1, 0); //����� ���� ��������
	int side = (int)(1024 / scale); //������ ������� ����� � ��������
	SDL_FRect pos = { x * side, y * side, side, side }; //�����, ���� �� ������ ���������� ����
	SDL_RenderTexture(renderer, fog, &i, &pos); //������
}

void drawMap() {
	for (int x = 0; x < globalMap->globalMapSideSize; x++) {
		for (int y = 0; y < globalMap->globalMapSideSize; y++) {
			if (globalMap->globalMap[x + globalMap->globalMapSideSize * y] == LOWLANDS) {
				drawFog(5, x, y);
			}
			else if (globalMap->globalMap[x + globalMap->globalMapSideSize * y] == MOUNTAINS) {
				drawTile(STONE, x, y);
			}
			else {
				drawTile(GRASS, x, y);
			}
		}
	}
}

void drawFrame(const arr3d<Tile, 64, 64, 256>* arr, int z) { //����� �� ����������, ��� ����, ������ GAME_SCREEN
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			if ((*arr)[z][x][y] != AIR) {//���� �� ���� ����� ���� ����, ���������� ���
				drawTile((*arr)[z][x][y], x, y);
			}
			else {
				bool flag = true;
				for (int i = 1; i <= 5; i++) { //����� �������� ������
					if ((*arr)[z - i][x][y] != AIR) {
						drawTile((*arr)[z - i][x][y], x, y);
						drawFog(i, x, y);
						flag = false;
						break;
						
					}
				
				}
				if (flag) {
					drawFog(5, x, y);
				}
			}
		}
	}
}

void drawMainMenu() { //������ ������� ����

	//������ ������ �����, ���������� ����� � ����������
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	
};