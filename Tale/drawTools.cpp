#include "SDL.h"
#include "enums.h"
#include "templates.h"
#include "localMap.h"
#include <iostream>

static SDL_Renderer* renderer = NULL;//renderer
static GlobalMap* globalMap;

static SDL_Texture* placeholder = NULL;//текстура заполнитель
static SDL_Texture* betaTileset = NULL;//тестовый тайлсет     ===> заменить все на 32х32 когда нибудь
static SDL_Texture* rat16 = NULL; //мыш
static SDL_Texture* fog = NULL;//туманъ

static int scale = 64; //в будущем зум сделать кастомизируемым, сколько тайлов помещается на экран
static int screenWidth;
static int screenHeight;

static SDL_FRect cut(int x, int y) { //функция, находит нужный квадратик тайлсета по его координатам на пнгшке
	return { (float)(32 * x), (float)(32 * y), 32.f, 32.f };
};

void drawToolsInit(SDL_Renderer* rend, GlobalMap* map, int w, int h) { //функция, чтобы передать всё нужное из main сюда
	renderer = rend;
	globalMap = map;
	placeholder = IMG_LoadTexture(renderer, "Assets/coconut.png"); //инициализирую текстуры
	betaTileset = IMG_LoadTexture(renderer, "Assets/ts_simple32.png");
	rat16 = IMG_LoadTexture(renderer, "Assets/rat.png");
	fog = IMG_LoadTexture(renderer, "Assets/ts_fog.png");
	screenWidth = w;
	screenHeight = h;
};

static void drawTile(Tile tile, int x, int y) { //нарисовать тайл
	SDL_FRect i; //какой тайл рисовать
	int side = (int)(1024 / scale); //длинна стороны тайла в пикселях
	SDL_FRect pos = { x*side, y*side, side, side }; //место, куда на экране нарисовать тайл
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
	SDL_RenderTexture(renderer, betaTileset, &i, &pos); //рисует
};

static void drawFog(int lvl, int x, int y) {
	SDL_FRect i = cut(lvl-1, 0); //какой тайл рисовать
	int side = (int)(1024 / scale); //длинна стороны тайла в пикселях
	SDL_FRect pos = { x * side, y * side, side, side }; //место, куда на экране нарисовать тайл
	SDL_RenderTexture(renderer, fog, &i, &pos); //рисует
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

void drawFrame(const arr3d<Tile, 64, 64, 256>* arr, int z) { //потом всё перекопать, это тест, рисует GAME_SCREEN
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			if ((*arr)[z][x][y] != AIR) {//если на этом месте есть тайл, нарисовать его
				drawTile((*arr)[z][x][y], x, y);
			}
			else {
				bool flag = true;
				for (int i = 1; i <= 5; i++) { //иначе рисовать туманы
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

void drawMainMenu() { //рисует главное меню

	//рисует кнопку старт, переделать потом с текстурами
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	
};