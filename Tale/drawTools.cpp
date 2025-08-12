#include "SDL.h"
#include "enums.h"
#include "templates.h"
#include "localMap.h"
#include "projectData.h"
#include <iostream>

static Data* data;

static SDL_Texture* placeholder = NULL;//текстура заполнитель
static SDL_Texture* betaTileset = NULL;//тестовый тайлсет     ===> заменить все на 32х32 когда нибудь
static SDL_Texture* rat16 = NULL; //мыш
static SDL_Texture* fog = NULL;//туманъ
static SDL_Texture* buttonStart = NULL;

static SDL_FRect cut(int x, int y) { //функция, находит нужный квадратик тайлсета по его координатам на пнгшке
	return { (float)(32 * x), (float)(32 * y), 32.f, 32.f };
};

void drawTools_getData(Data *d) {
	data = d;

	placeholder = IMG_LoadTexture(data->renderer, "Assets/coconut.png"); //инициализирую текстуры
	betaTileset = IMG_LoadTexture(data->renderer, "Assets/ts_simple32.png");
	rat16 = IMG_LoadTexture(data->renderer, "Assets/rat.png");
	fog = IMG_LoadTexture(data->renderer, "Assets/ts_fog.png");
	buttonStart = IMG_LoadTexture(data->renderer, "Assets/button_start.png");
	data->menus->menus[MAIN_MENU].buttons["start"].texture = buttonStart;
}

static void drawTile(Tile tile, int x, int y) { //нарисовать тайл
	SDL_FRect i; //какой тайл рисовать
	SDL_FRect pos = { x*data->side + data->startOffsetX, y*data->side + data->startOffsetY, data->side, data->side }; //место, куда на экране нарисовать тайл
	switch (tile) { //выбирает тайл для рисовки и срезает его с тайлсета
	case AIR:
		break;
	case GRASS:
		i = cut(0, 0);
		break;
	case STONE: //текстура говна, не камня
		i = cut(1, 0);
		break;

	};
	SDL_RenderTexture(data->renderer, betaTileset, &i, &pos); //рисует
};

static void drawFog(int lvl, int x, int y) {
	SDL_FRect i = cut(lvl-1, 0); //какой тайл рисовать
	SDL_FRect pos = { x * data->side + data->startOffsetX, y * data->side + data->startOffsetY, data->side, data->side }; //место, куда на экране нарисовать тайл
	SDL_RenderTexture(data->renderer, fog, &i, &pos); //рисует
}

void drawMap() {
	for (int x = data->zoomStartX; x < (data->scale + data->zoomStartX); x++) {//подразумевается, что карту можно зумить
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
	for (int x = 0; x < data->globalMap->regionalMapSideSize; x++) {//пока рег. карту зумить нельзя
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

void drawFrame() { //потом всё перекопать, это тест, рисует GAME_SCREEN
	int z = data->globalMap->viewedZLevel;
	for (int x = 0; x < data->scale; x++) {
		for (int y = 0; y < data->scale; y++) {
			int X = x + data->zoomStartX;
			int Y = y + data->zoomStartY;
			if ((*data->globalMap->tileMap)[z][X][Y] != AIR) {//если на этом месте есть тайл, нарисовать его
				drawTile((*data->globalMap->tileMap)[z][X][Y], x, y);
			}
			else {
				bool flag = true;//flag - есть ли блок ближе 5-ти тайлов вниз
				for (int i = 1; i <= 5; i++) { //иначе рисовать туманы
					if ((*data->globalMap->tileMap)[z - i][X][Y] != AIR) {//если наткнулся на блок
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

void drawMainMenu() { //рисует главное меню
	//рисует
	for (const auto &pair : data->menus->menus[MAIN_MENU].buttons) {
		SDL_FRect rect = { pair.second.x, pair.second.y, pair.second.width, pair.second.height };//куда рисовать
		SDL_RenderTexture(data->renderer, pair.second.texture, NULL, &rect);
	}
};