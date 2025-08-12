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
static SDL_Texture* buttonStart = NULL;

static SDL_FRect cut(int x, int y) { //�������, ������� ������ ��������� �������� �� ��� ����������� �� ������
	return { (float)(32 * x), (float)(32 * y), 32.f, 32.f };
};

void drawTools_getData(Data *d) {
	data = d;

	placeholder = IMG_LoadTexture(data->renderer, "Assets/coconut.png"); //������������� ��������
	betaTileset = IMG_LoadTexture(data->renderer, "Assets/ts_simple32.png");
	rat16 = IMG_LoadTexture(data->renderer, "Assets/rat.png");
	fog = IMG_LoadTexture(data->renderer, "Assets/ts_fog.png");
	buttonStart = IMG_LoadTexture(data->renderer, "Assets/button_start.png");
	data->menus->menus[MAIN_MENU].buttons["start"].texture = buttonStart;
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
	for (int x = data->zoomStartX; x < (data->scale + data->zoomStartX); x++) {//���������������, ��� ����� ����� ������
		for (int y = data->zoomStartY; y < (data->scale + data->zoomStartY); y++) {
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

void drawRegionalMap() {
	for (int x = 0; x < data->globalMap->regionalMapSideSize; x++) {//���� ���. ����� ������ ������
		for (int y = 0; y < data->globalMap->regionalMapSideSize; y++) {
			if (data->globalMap->regionalMap[x + data->globalMap->regionalMapSideSize * y] == LOWLANDS) {
				drawFog(5, x, y);
			}
			else if (data->globalMap->regionalMap[x + data->globalMap->regionalMapSideSize * y] == MOUNTAINS) {
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
	for (int x = 0; x < data->scale; x++) {
		for (int y = 0; y < data->scale; y++) {
			int X = x + data->zoomStartX;
			int Y = y + data->zoomStartY;
			if ((*data->globalMap->tileMap)[z][X][Y] != AIR) {//���� �� ���� ����� ���� ����, ���������� ���
				drawTile((*data->globalMap->tileMap)[z][X][Y], x, y);
			}
			else {
				bool flag = true;//flag - ���� �� ���� ����� 5-�� ������ ����
				for (int i = 1; i <= 5; i++) { //����� �������� ������
					if ((*data->globalMap->tileMap)[z - i][X][Y] != AIR) {//���� ��������� �� ����
						drawTile((*data->globalMap->tileMap)[z - i][X][Y], x, y);
						drawFog(i, x, y);
						flag = false;
						break;
						
					}
				
				}
				if (flag) {
					drawFog(5, x, y);
				}
			}
		}//!
	}
}

void drawMainMenu() { //������ ������� ����
	//������
	for (const auto &pair : data->menus->menus[MAIN_MENU].buttons) {
		SDL_FRect rect = { pair.second.x, pair.second.y, pair.second.width, pair.second.height };//���� ��������
		SDL_RenderTexture(data->renderer, pair.second.texture, NULL, &rect);
	}
};