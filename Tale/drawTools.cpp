#include "SDL.h"
#include "enums.h"
#include "templates.h"
#include "localMap.h"
#include "projectData.h"
#include <iostream>

static Data* data;

static SDL_Texture* placeholder = NULL;//�������� �����������
static SDL_Texture* betaTileset = NULL;//�������� �������     ===> �������� ��� �� 32�32 ����� ������
static SDL_Texture* rat16 = NULL; //���
static SDL_Texture* fog = NULL;//������

static SDL_FRect cut(int x, int y) { //�������, ������� ������ ��������� �������� �� ��� ����������� �� ������
	return { (float)(32 * x), (float)(32 * y), 32.f, 32.f };
};

void drawTools_getData(Data *d) {
	data = d;

	placeholder = IMG_LoadTexture(data->renderer, "Assets/coconut.png"); //������������� ��������
	betaTileset = IMG_LoadTexture(data->renderer, "Assets/ts_simple32.png");
	rat16 = IMG_LoadTexture(data->renderer, "Assets/rat.png");
	fog = IMG_LoadTexture(data->renderer, "Assets/ts_fog.png");


}

static void drawTile(Tile tile, int x, int y) { //���������� ����
	SDL_FRect i; //����� ���� ��������
	SDL_FRect pos = { x*data->side + data->startOffsetX, y*data->side + data->startOffsetY, data->side, data->side }; //�����, ���� �� ������ ���������� ����
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
	SDL_RenderTexture(data->renderer, betaTileset, &i, &pos); //������
};

static void drawFog(int lvl, int x, int y) {
	SDL_FRect i = cut(lvl-1, 0); //����� ���� ��������
	SDL_FRect pos = { x * data->side + data->startOffsetX, y * data->side + data->startOffsetY, data->side, data->side }; //�����, ���� �� ������ ���������� ����
	SDL_RenderTexture(data->renderer, fog, &i, &pos); //������
}

void drawMap() {
	for (int x = 0; x < data->globalMap->globalMapSideSize; x++) {
		for (int y = 0; y < data->globalMap->globalMapSideSize; y++) {
			if (data->globalMap->globalMap[x + data->globalMap->globalMapSideSize * y] == LOWLANDS) {
				drawFog(5, x, y);
			}
			else if (data->globalMap->globalMap[x + data->globalMap->globalMapSideSize * y] == MOUNTAINS) {
				drawTile(STONE, x, y);
			}
			else {
				drawTile(GRASS, x, y);
			}
		}
	}
}

void drawFrame() { //����� �� ����������, ��� ����, ������ GAME_SCREEN
	int z = data->globalMap->viewedZLevel;
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			if ((*data->globalMap->tileMap)[z][x][y] != AIR) {//���� �� ���� ����� ���� ����, ���������� ���
				drawTile((*data->globalMap->tileMap)[z][x][y], x, y);
			}
			else {
				bool flag = true;//flag - ���� �� ���� ����� 5-�� ������ ����
				for (int i = 1; i <= 5; i++) { //����� �������� ������
					if ((*data->globalMap->tileMap)[z - i][x][y] != AIR) {//���� ��������� �� ����
						drawTile((*data->globalMap->tileMap)[z - i][x][y], x, y);
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
	int screenWidth = data->windowWidth;
	int screenHeight = data->windowHeight;
	SDL_SetRenderDrawColor(data->renderer, 255, 255, 255, 255);
	SDL_RenderLine(data->renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20));
	SDL_RenderLine(data->renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(data->renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(data->renderer, (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	
};