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

	//������� ������ ����� ������� ����� �� ����� ����, ������ ����� ���
	switch (data->screen) {
	case MAIN_MENU: {
		if ((x > 0.45 * data->windowWidth) and (x < 0.55 * data->windowWidth) and (y > 0.45 * data->windowHeight) and (y < 0.55 * data->windowHeight)) {
			data->globalMap->seed = generateSeed();
			firstGeneratingSequence();
			data->screen = GAME_SCREEN_GLOBAL_MAP;
		}
		break;
	}
	case GAME_SCREEN_GLOBAL_MAP: {//��������� � ���������� �����
		//���� ��� � �������� �������� ������
		if ((x > data->startOffsetX and x < (data->windowWidth - data->startOffsetX)) and (y > data->startOffsetY and y < (data->windowHeight - data->startOffsetY))) {
			x = (int)((x - data->startOffsetX) / data->side);
			y = (int)((y - data->startOffsetY) / data->side);//������ x � y - ���������� ���������� �����
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
		if (key == SDLK_R) { //���������� �� ������� �����
			if (data->globalMap->viewedZLevel < 256) {
				data->globalMap->viewedZLevel++;
			}
		}
		else if (key == SDLK_F) {// ���������� �� ������� ����
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
		if (key == SDLK_L) { //���������
			data->globalMap->seed = load();
			firstGeneratingSequence();
			data->screen = GAME_SCREEN_GLOBAL_MAP;
		}
		break;
	}


	}
}