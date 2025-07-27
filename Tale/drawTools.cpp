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

static SDL_FRect cut(int x, int y) { //функция, находит нужный квадратик тайлсета по его координатам на пнгшке
	return { (float)(32 * x), (float)(32 * y), 32.f, 32.f };
};

void drawTools_getData(Data *d) {
	data = d;

	placeholder = IMG_LoadTexture(data->renderer, "Assets/coconut.png"); //инициализирую текстуры
	betaTileset = IMG_LoadTexture(data->renderer, "Assets/ts_simple32.png");
	rat16 = IMG_LoadTexture(data->renderer, "Assets/rat.png");
	fog = IMG_LoadTexture(data->renderer, "Assets/ts_fog.png");


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

void drawFrame() { //потом всё перекопать, это тест, рисует GAME_SCREEN
	int z = data->globalMap->viewedZLevel;
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			if ((*data->globalMap->tileMap)[z][x][y] != AIR) {//если на этом месте есть тайл, нарисовать его
				drawTile((*data->globalMap->tileMap)[z][x][y], x, y);
			}
			else {
				bool flag = true;//flag - есть ли блок ближе 5-ти тайлов вниз
				for (int i = 1; i <= 5; i++) { //иначе рисовать туманы
					if ((*data->globalMap->tileMap)[z - i][x][y] != AIR) {//если наткнулся на блок
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

void drawMainMenu() { //рисует главное меню

	//рисует кнопку старт, переделать потом с текстурами
	int screenWidth = data->windowWidth;
	int screenHeight = data->windowHeight;
	SDL_SetRenderDrawColor(data->renderer, 255, 255, 255, 255);
	SDL_RenderLine(data->renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20));
	SDL_RenderLine(data->renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(data->renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(data->renderer, (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	
};