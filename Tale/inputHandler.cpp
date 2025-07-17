#include "SDL.h"
#include "enums.h"
#include "localMap.h"
#include "saveFileHandler.h"
#include "temporary.h"
#include "generators.h"
#include <iostream>

static int windowWidth;
static int windowHeight;
static GlobalMap* map;

void inputHandlerInit(GlobalMap* m, int w, int h) {
	map = m;
	windowWidth = w;
	windowHeight = h;
}

void mouseInput(SDL_Event* event, Screen &screen) {
	int x = event->button.x;
	int y = event->button.y;

	//нажатие кнопки старт пускает сразу на экран игры, создаёт новый мир
	if ((x > 0.45 * windowWidth) and (x < 0.55 * windowWidth) and (y > 0.45 * windowHeight) and (y < 0.55 * windowHeight)) {
		map->seed = generateSeed();
		firstGeneratingSequence();
		//map->tileMap = generateOnHeightMap(map->seed);//!!!
		screen = GAME_SCREEN;
	}
}

void keyboardInput(SDL_Event* event, Screen &screen) {
	switch (screen) {

	case GAME_SCREEN: {
		SDL_Keycode key = event->key.key;
		if (key == SDLK_R) { //посмотреть на уровень вверх
			map->viewedZLevel++;
		}
		else if (key == SDLK_F) {// посмотреть на уровень вниз
			map->viewedZLevel--;
		}
		else if (key == SDLK_Q) {//перезагрузить мир, убрать потом
			map->tileMap = generateOnHeightMap(map->seed);
			map->viewedZLevel = 128;
		}
		break;
	}

	case MAIN_MENU: {
		SDL_Keycode key = event->key.key;
		if (key == SDLK_L) {
			map->seed = load();
			firstGeneratingSequence();
			//map->tileMap = generateOnHeightMap(map->seed);//сгенерировать карту по имеющемуся сиду
			screen = GAME_SCREEN;
		}
		break;
	}

	}
}