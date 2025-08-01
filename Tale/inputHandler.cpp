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

	//������� ������ ����� ������� ����� �� ����� ����, ������ ����� ���

	switch (data->screen) {
	case MAIN_MENU: {
		if (data->menus.menus[MAIN_MENU].buttons["start"].isInbound(x, y)) {
			data->menus.menus[MAIN_MENU].buttons["start"].action();
		}
		break;
	}
	case GAME_SCREEN_GLOBAL_MAP: {//��������� � ���������� �����
		//���� ��� � �������� �������� ������
		if (data->menus.menus[GAME_SCREEN_GLOBAL_MAP].buttons["map"].isInbound(x, y)) {
			data->globalMap->viewedChunkX = (int)((x - data->startOffsetX) / data->side);
			data->globalMap->viewedChunkY = (int)((y - data->startOffsetY) / data->side);//���������� ���������� �����
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
		else if (key == SDLK_M) {//������� ���������� �����
			data->screen = GAME_SCREEN_GLOBAL_MAP;
		}
		else if (key == SDLK_UP) {//������� ���� ������
			if (data->globalMap->viewedChunkY > 0) {
				data->globalMap->viewedChunkY--;
				createChunk();
			}
		}
		else if (key == SDLK_DOWN) {//������� ���� ������
			if (data->globalMap->viewedChunkY <63) {
				data->globalMap->viewedChunkY++;
				createChunk();
			}
		}
		else if (key == SDLK_LEFT) {//������� ���� ������
			if (data->globalMap->viewedChunkX > 0) {
				data->globalMap->viewedChunkX--;
				createChunk();
			}
		}
		else if (key == SDLK_RIGHT) {//������� ���� ������
			if (data->globalMap->viewedChunkX < 63) {
				data->globalMap->viewedChunkX++;
				createChunk();
			}
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