#include "SDL.h"
#include "enums.h"


static SDL_Renderer* renderer = NULL;//renderer

static SDL_Texture* placeholder = NULL;//текстура пустышка
static SDL_Texture* betaTileset = NULL;//тестовый тайлсет     ===> заменить все на 32х32 когда нибудь
static SDL_Texture* rat16 = NULL; //мыш

static int scale = 64; //в будущем зум сделать кастомизируемым

static SDL_FRect cut(int x, int y) { //функция, находит нужный квадратик тайлсета по его координатам на пнгшке
	return { (float)(16 * x), (float)(16 * y), 16.f, 16.f };
};

void drawToolsInit(SDL_Renderer* rend) { //функция, чтобы передать всё нужное из main сюда
	renderer = rend;
	
	placeholder = IMG_LoadTexture(renderer, "Assets/coconut.png"); //инициализирую текстуры
	betaTileset = IMG_LoadTexture(renderer, "Assets/ts_simple.png");
	rat16 = IMG_LoadTexture(renderer, "Assets/rat.png");
};

void drawTile(Tile tile, int x, int y) { //нарисовать тайл
	SDL_FRect i; //какой тайл рисовать
	int side = (int)(1024 / scale); //длинна стороны тайла в пикселях
	SDL_FRect pos = { x*side, y*side, side, side }; //место, куда на экране нарисовать тайл
	switch (tile) { //выбирает тайл для рисовки и срезает его с тайлсета
	case GRASS:
		i = cut(1, 0);
		break;
	case STONE:
		i = cut(2, 0);
		break;

	};
	SDL_RenderTexture(renderer, betaTileset, &i, &pos); //рисует
};

void drawFrame() { //потом всё перекопать, это тест
	for (int x = 0; x < scale; x++) {
		for (int y = 0; y < scale; y++) {
			int r = SDL_rand(2);
			if (r == 1) {
				drawTile(GRASS, x, y);
			}
			else {
				drawTile(STONE, x, y);
			}
		};
	};
}
