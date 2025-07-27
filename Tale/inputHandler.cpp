#include "SDL.h"
#include "enums.h"
#include "localMap.h"
#include "saveFileHandler.h"
#include "temporary.h"
#include "generators.h"
#include "projectData.h"
#include "generators.h"
#include <iostream>

static Data* data;

void inputHandler_getData(Data* d) {
	data = d;
}

void mouseInput(SDL_Event* event) {

	int x = event->button.x;
	int y = event->button.y;

	//нажатие кнопки старт пускает сразу на экран игры, создаёт новый мир
	switch (data->screen) {
	case MAIN_MENU: {
		if ((x > 0.45 * data->windowWidth) and (x < 0.55 * data->windowWidth) and (y > 0.45 * data->windowHeight) and (y < 0.55 * data->windowHeight)) {
			data->globalMap->seed = generateSeed();
			firstGeneratingSequence();
			data->screen = GAME_SCREEN_GLOBAL_MAP;
		}
		break;
	}
	case GAME_SCREEN_GLOBAL_MAP: {//перенести в генераторы позже
		//если тык в пределах игрового экрана
		if ((x > data->startOffsetX and x < (data->windowWidth - data->startOffsetX)) and (y > data->startOffsetY and y < (data->windowHeight - data->startOffsetY))) {
			x = (int)((x - data->startOffsetX) / data->side);
			y = (int)((y - data->startOffsetY) / data->side);//теперь x и y - координаты кликнутого тайла
			createChunk(x, y);
			data->screen = GAME_SCREEN;
		}
		break;
	}
	} 
}

void keyboardInput(SDL_Event* event) {
	switch (data->screen) {

	case GAME_SCREEN: {
		SDL_Keycode key = event->key.key;
		if (key == SDLK_R) { //посмотреть на уровень вверх
			if (data->globalMap->viewedZLevel < 256) {
				data->globalMap->viewedZLevel++;
			}
		}
		else if (key == SDLK_F) {// посмотреть на уровень вниз
			if (data->globalMap->viewedZLevel > 0) {
				data->globalMap->viewedZLevel--;
			}
		}
		else if (key == SDLK_Q) {
			data->screen = GAME_SCREEN_GLOBAL_MAP;
		}
		break;
	}

	case MAIN_MENU: {
		SDL_Keycode key = event->key.key;
		if (key == SDLK_L) { //Загрузить
			data->globalMap->seed = load();
			firstGeneratingSequence();
			data->screen = GAME_SCREEN_GLOBAL_MAP;
		}
		break;
	}


	}
}