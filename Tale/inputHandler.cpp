#include "SDL.h"
#include "enums.h"
#include "localMap.h"
#include "saveFileHandler.h"
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
		if (data->menus.menus[MAIN_MENU].buttons["start"].isInbound(x, y)) {
			data->menus.menus[MAIN_MENU].buttons["start"].action();
		}
		break;
	}
	case GAME_SCREEN_GLOBAL_MAP: {//перенести в генераторы позже
		//если тык в пределах игрового экрана
		if (data->menus.menus[GAME_SCREEN_GLOBAL_MAP].buttons["map"].isInbound(x, y)) {
			data->globalMap->viewedChunkX = (int)((x - data->startOffsetX) / data->side);
			data->globalMap->viewedChunkY = (int)((y - data->startOffsetY) / data->side);//координаты кликнутого тайла
			data->menus.menus[GAME_SCREEN_GLOBAL_MAP].buttons["map"].action();
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
		else if (key == SDLK_M) {//открыть глобальную карту
			data->screen = GAME_SCREEN_GLOBAL_MAP;
		}
		else if (key == SDLK_UP) {//открыть чанк сверху
			if (data->globalMap->viewedChunkY > 0) {
				data->globalMap->viewedChunkY--;
				createChunk();
			}
		}
		else if (key == SDLK_DOWN) {//открыть чанк сверху
			if (data->globalMap->viewedChunkY <63) {
				data->globalMap->viewedChunkY++;
				createChunk();
			}
		}
		else if (key == SDLK_LEFT) {//открыть чанк сверху
			if (data->globalMap->viewedChunkX > 0) {
				data->globalMap->viewedChunkX--;
				createChunk();
			}
		}
		else if (key == SDLK_RIGHT) {//открыть чанк сверху
			if (data->globalMap->viewedChunkX < 63) {
				data->globalMap->viewedChunkX++;
				createChunk();
			}
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