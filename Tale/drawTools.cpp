#include "SDL.h"
#include "enums.h"
#include "templates.h"

static SDL_Renderer* renderer = NULL;//renderer

static SDL_Texture* placeholder = NULL;//текстура заполнитель
static SDL_Texture* betaTileset = NULL;//тестовый тайлсет     ===> заменить все на 32х32 когда нибудь
static SDL_Texture* rat16 = NULL; //мыш

static int scale = 64; //в будущем зум сделать кастомизируемым, сколько тайлов помещается на экран
static int screenWidth;
static int screenHeight;

static SDL_FRect cut(int x, int y) { //функция, находит нужный квадратик тайлсета по его координатам на пнгшке
	return { (float)(32 * x), (float)(32 * y), 32.f, 32.f };
};

void drawToolsInit(SDL_Renderer* rend, int w, int h) { //функция, чтобы передать всё нужное из main сюда
	renderer = rend;
	
	placeholder = IMG_LoadTexture(renderer, "Assets/coconut.png"); //инициализирую текстуры
	betaTileset = IMG_LoadTexture(renderer, "Assets/ts_simple32.png");
	rat16 = IMG_LoadTexture(renderer, "Assets/rat.png");

	screenWidth = w;
	screenHeight = h;
};

void drawTile(Tile tile, int x, int y) { //нарисовать тайл
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

void drawFrame(const arr3d<Tile, 64, 64, 256>* arr, int z) { //потом всё перекопать, это тест, рисует GAME_SCREEN
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			drawTile((*arr)[z][x][y], x, y);
		};
	};
};

void drawMainMenu() { //рисует главное меню

	//рисует кнопку старт, переделать потом с текстурами
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	
};