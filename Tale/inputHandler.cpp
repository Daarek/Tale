#include "SDL.h"
#include "enums.h"
#include "localMap.h"
#include "saveFileHandler.h"
#include "generators.h"
#include "projectData.h"
#include "generators.h"
#include "actions.h"
#include <iostream>

static Data* data;
static Tile hold = STONE;

void inputHandler_getData(Data* d) {
	data = d;
}

void mouseInput(SDL_Event* event) {

	int x = event->button.x;
	int y = event->button.y;

	//нажатие кнопки старт пускает сразу на экран игры, создаёт новый мир

	switch (data->screen) {
	case MAIN_MENU: {
		if (data->menus->menus[MAIN_MENU].buttons["start"].isInbound(x, y)) {
			data->menus->menus[MAIN_MENU].buttons["start"].action();
		}
		break;
	}
	case GAME_SCREEN_GLOBAL_MAP: {//перенести в генераторы позже
		//если тык в пределах игрового экрана
		if (data->menus->menus[GAME_SCREEN_GLOBAL_MAP].buttons["map"].isInbound(x, y)) {
			data->globalMap->viewedChunkX = (int)((x - data->startOffsetX) / data->side);
			data->globalMap->viewedChunkY = (int)((y - data->startOffsetY) / data->side);//координаты кликнутого тайла
			data->globalMap->viewedChunkX -= (data->globalMap->viewedChunkX == data->globalMap->globalMapSideSize);//профилактика бага
			data->globalMap->viewedChunkY -= (data->globalMap->viewedChunkY == data->globalMap->globalMapSideSize);
			data->menus->menus[GAME_SCREEN_GLOBAL_MAP].buttons["map"].action();
		}
		break;
	}
	case GAME_SCREEN: {
		int X = (int)((x - data->startOffsetX) / data->side);
		int Y = (int)((y - data->startOffsetY) / data->side);
		X -= (X == 64);//профилактика бага
		Y -= (Y == 64);
		placeBlock(hold, X, Y, data->globalMap->viewedZLevel);
		break;
	}
	case GAME_SCREEN_REGIONAL_MAP: {
		if (data->menus->menus[GAME_SCREEN_REGIONAL_MAP].buttons["map"].isInbound(x, y)) {
			data->globalMap->viewedRegChunkX = (int)((x - data->startOffsetX) / data->side);
			data->globalMap->viewedRegChunkY = (int)((y - data->startOffsetY) / data->side);//координаты кликнутого тайла
			data->globalMap->viewedRegChunkX -= (data->globalMap->viewedRegChunkX == data->globalMap->regionalMapSideSize);//профилактика бага
			data->globalMap->viewedRegChunkY -= (data->globalMap->viewedRegChunkY == data->globalMap->regionalMapSideSize);
			data->menus->menus[GAME_SCREEN_REGIONAL_MAP].buttons["map"].action();
		}
		break;
	}
	} 
	
}

void keyboardInput(SDL_Event* event) {
	SDL_Keycode key = event->key.key;
	switch (data->screen) {

	case GAME_SCREEN: {
		
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
			data->screen = GAME_SCREEN_REGIONAL_MAP;
			data->scale = data->globalMap->regionalMapSideSize;
			data->side = (int)(data->gameScreenWidth / data->scale);
		}
		else if (key == SDLK_UP) {//открыть чанк сверху
			if (data->globalMap->viewedRegChunkY > 0) {
				data->globalMap->viewedRegChunkY--;
				createChunk();
			}
		}
		else if (key == SDLK_DOWN) {//открыть чанк сверху
			if (data->globalMap->viewedRegChunkY < data->globalMap->regionalMapSideSize-1) {
				data->globalMap->viewedRegChunkY++;
				createChunk();
			}
		}
		else if (key == SDLK_LEFT) {//открыть чанк сверху
			if (data->globalMap->viewedRegChunkX > 0) {
				data->globalMap->viewedRegChunkX--;
				createChunk();
			}
		}
		else if (key == SDLK_RIGHT) {//открыть чанк сверху
			if (data->globalMap->viewedRegChunkX < data->globalMap->regionalMapSideSize-1) {
				data->globalMap->viewedRegChunkX++;
				createChunk();
			}
		}
		else if (key == SDLK_0) {//test
			hold = AIR;
		}
		else if (key == SDLK_1) {
			hold = GRASS;
		}
		else if (key == SDLK_2) {
			hold = STONE;
		}
		else if (key == SDLK_W) {//камеру поднять
			if (data->zoomStartY > 0) {
				data->zoomStartY--;
			}
		}
		else if (key == SDLK_A) {//камеру влево
			if (data->zoomStartX > 0) {
				data->zoomStartX--;
			}
		}
		else if (key == SDLK_S) {//камеру вниз
			if (data->zoomStartY < (64 - data->scale)) {
				data->zoomStartY++;
			}
		}
		else if (key == SDLK_D) {//камеру вправо
			if (data->zoomStartX < (64 - data->scale)) {
				data->zoomStartX++;
			}
		}
		else if (key == SDLK_T) {//zoom in
			if (data->scale > 1) {
				data->scale--;
				data->side = (int)(data->gameScreenWidth / data->scale);
			}
		}
		else if (key == SDLK_G) {//zoom out
			if (data->scale < 64) {
				data->scale++;
				data->side = (int)(data->gameScreenWidth / data->scale);

				if (data->zoomStartX > 0) {//профилактика выхода за пределы массива
					data->zoomStartX--;
				}
				if (data->zoomStartY > 0) {
					data->zoomStartY--;
				}
			}
		}
		break;
	}

	case MAIN_MENU: {
		if (key == SDLK_L) { //Загрузить
			data->globalMap->seed = load();
			firstGeneratingSequence();
			data->screen = GAME_SCREEN_GLOBAL_MAP;
		}
		break;
	}

	case GAME_SCREEN_REGIONAL_MAP: {
		if (key == SDLK_UP) {//открыть чанк сверху
			if (data->globalMap->viewedChunkY > 0) {
				data->globalMap->viewedChunkY--;
				createRegionalChunk();
			}
		}
		else if (key == SDLK_DOWN) {//открыть чанк снизу
			if (data->globalMap->viewedChunkY < data->globalMap->globalMapSideSize - 1) {
				data->globalMap->viewedChunkY++;
				createRegionalChunk();
			}
		}
		else if (key == SDLK_LEFT) {//открыть чанк cлева
			if (data->globalMap->viewedChunkX > 0) {
				data->globalMap->viewedChunkX--;
				createRegionalChunk();
			}
		}
		else if (key == SDLK_RIGHT) {//открыть чанк справа
			if (data->globalMap->viewedChunkX < data->globalMap->globalMapSideSize - 1) {
				data->globalMap->viewedChunkX++;
				createRegionalChunk();
			}
		}
		else if (key == SDLK_M) {//открыть глобальную карту
			data->screen = GAME_SCREEN_GLOBAL_MAP;
			data->scale = data->globalMap->globalMapSideSize;
			data->side = (int)(data->gameScreenWidth / data->scale);
		}
	}


	}
}