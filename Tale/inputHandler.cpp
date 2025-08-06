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

	//������� ������ ����� ������� ����� �� ����� ����, ������ ����� ���

	switch (data->screen) {
	case MAIN_MENU: {
		if (data->menus->menus[MAIN_MENU].buttons["start"].isInbound(x, y)) {
			data->menus->menus[MAIN_MENU].buttons["start"].action();
		}
		break;
	}
	case GAME_SCREEN_GLOBAL_MAP: {//��������� � ���������� �����
		//���� ��� � �������� �������� ������
		if (data->menus->menus[GAME_SCREEN_GLOBAL_MAP].buttons["map"].isInbound(x, y)) {
			data->globalMap->viewedChunkX = (int)((x - data->startOffsetX) / data->side);
			data->globalMap->viewedChunkY = (int)((y - data->startOffsetY) / data->side);//���������� ���������� �����
			data->menus->menus[GAME_SCREEN_GLOBAL_MAP].buttons["map"].action();
		}
		break;
	}
	case GAME_SCREEN: {
		int X = (int)((x - data->startOffsetX) / data->side);
		int Y = (int)((y - data->startOffsetY) / data->side);
		placeBlock(hold, X, Y, data->globalMap->viewedZLevel);
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
		else if (key == SDLK_0) {//test
			hold = AIR;
		}
		else if (key == SDLK_1) {
			hold = GRASS;
		}
		else if (key == SDLK_2) {
			hold = STONE;
		}
		else if (key == SDLK_W) {//������ �������
			if (data->zoomStartY > 0) {
				data->zoomStartY--;
			}
		}
		else if (key == SDLK_A) {//������ �����
			if (data->zoomStartX > 0) {
				data->zoomStartX--;
			}
		}
		else if (key == SDLK_S) {//������ ����
			if (data->zoomStartY < (64 - data->scale)) {
				data->zoomStartY++;
			}
		}
		else if (key == SDLK_D) {//������ ������
			if (data->zoomStartX < (64 - data->scale)) {
				data->zoomStartX++;
			}
		}
		else if (key == SDLK_T) {//zoom in
			if (data->scale > 1) {
				data->scale--;
				data->side = (int)(data->windowHeight / data->scale);
			}
		}
		else if (key == SDLK_G) {//zoom out
			if (data->scale < 64) {
				data->scale++;
				data->side = (int)(data->windowHeight / data->scale);

				if (data->zoomStartX > 0) {//������������ ������ �� ������� �������
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