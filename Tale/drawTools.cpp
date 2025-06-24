#include "SDL.h"
#include "enums.h"

static SDL_Renderer* renderer = NULL;//renderer

static SDL_Texture* placeholder = NULL;//текстура пустышка
static SDL_Texture* betaTileset = NULL;//тестовый тайлсет     ===> заменить все на 32х32 когда нибудь
static SDL_Texture* rat16 = NULL; //мыш

static SDL_FRect cut(int x) { //функция, находит нужный квадратик тайлсета по его координатам на пнгшке
	return { (float)(16 * x), 0.f, 16.f, 16.f };
};

void drawToolsInit(SDL_Renderer* rend) { //функция, чтобы передать всё нужное из main сюда
	renderer = rend;
	
	placeholder = IMG_LoadTexture(renderer, "Assets/coconut.png"); //инициализирую текстуры
	betaTileset = IMG_LoadTexture(renderer, "Assets/ts_simple.png");
	rat16 = IMG_LoadTexture(renderer, "Assets/rat.png");
};

void drawTile(Tile tile) { //нарисовать тайл
	SDL_FRect i;

	switch (tile) { //выбирает тайл для рисовки и срезает его с тайлсета
	case GRASS:
		i = cut(1);
		break;
	case STONE:
		i = cut(2);
		break;

	}
	SDL_RenderTexture(renderer, betaTileset, &i, NULL); //рисует

	SDL_RenderPresent(renderer); //удалить потом
};


