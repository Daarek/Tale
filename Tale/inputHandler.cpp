#include "SDL.h"
#include "enums.h"
#include "localMap.h"
#include "saveFileHandler.h"
#include "temporary.h"
#include "generators.h"
#include "projectData.h"
#include "generators.h"
#include <chrono>
#include <thread>
#include <iostream>

static Data* data;

void inputHandler_getData(Data* d) {
	data = d;
}

void mouseInput(SDL_Event* event) {
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

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
			x = (int)((x - data->startOffsetX) / data->scale);
			y = (int)((y - data->startOffsetY) / data->scale);//теперь x и y - координаты кликнутого тайла


			int t = 0;
			for (int i = 0; i < data->globalMap->octaveAmount; i++) {
				t = t + pow(2, i);
			}
			int low = (t / pow(2, (data->globalMap->octaveAmount - 1))); //число, на которое стоит поделить все высоты после создания полной сетки со всеми слоями

			float* heightMap = new float[4096];//карта высот для 1 маленького чанка, 64х64 = 4096
			for (int i = 0; i < 4096; i++) {//зануляю
				heightMap[i] = 0.0f;
			}

			for (int octave = 1; octave <= data->globalMap->octaveAmount; octave++) { //создавать карту по октаве

				int l = (int)(pow(2, octave - 1) * data->globalMap->initSize + 1);//длинна стороны одной октавы
				float* stackGrid = new float[(int)(pow(l, 2))]; //текущая октава
				for (int i = 0; i < (int)(pow(l, 2)); i++) {//копируем октаву
					stackGrid[i] = data->globalMap->perlinGrid[data->globalMap->getOctaveOffset(octave - 1) + i];
				}

				float step = pow(data->globalMap->initSize, octave) / data->globalMap->globalMapSideSize; //скольки координатам на октаве соответствует координата на глобальной карте
				float mStep = step / 64; ////скольки координатам на октаве соответствует координата на чанке

				for (int mx = 0; mx < 64; mx++) {//создаём высотную карту, mx и my - координаты тайлов в чанке
					for (int my = 0; my < 64; my++) {
						heightMap[mx + my * 64] += getNoiseValue(x * step + mx * mStep, y * step + my * mStep, stackGrid, l-1) / pow(2, octave - 1);
					}
				}

				delete[](stackGrid);
			}
			for (int i = 0; i < (int)pow(data->globalMap->globalMapSideSize, 2); i++) {//конвертирую в высоты в кубах
				heightMap[i] = (int)(heightMap[i] / low * 49 + 128);
				
			}

			for (x = 0; x < 64; x++) {
				for (y = 0; y < 64; y++) {
					for (int z = 0; z < 256; z++) {
						if (z < heightMap[x + 64 * y]) {
							(*data->globalMap->tileMap)[z][x][y] = STONE;
						}
						else if (z == heightMap[x + 64 * y]) {
							(*data->globalMap->tileMap)[z][x][y] = GRASS;
						}
						else {
							(*data->globalMap->tileMap)[z][x][y] = AIR;
						}
					}
				}
			}
			delete[](heightMap);
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
			data->globalMap->viewedZLevel--;
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