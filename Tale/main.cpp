#include "SDL.h"
#define	SDL_MAIN_USE_CALLBACKS
#include "SDL3/SDL_main.h"
#include "enums.h"
#include "drawTools.h"
#include "localMap.h"
#include "temporary.h"
#include <string>
#include <iostream>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
LocalMap* map = new LocalMap;
static int Z; //Z уровень карты, убрать потом
static Screen screen = MAIN_MENU;
static int screenWidth = 1920;
static int screenHeight = 1080;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
	
	map->tileMap = generateRandom();//!!!
	Z = 128; //!!!

	SDL_Init(SDL_INIT_VIDEO); //инициализация всего
	window = SDL_CreateWindow("main", screenWidth, screenHeight, NULL);
	renderer = SDL_CreateRenderer(window, NULL);
	drawToolsInit(renderer, screenWidth, screenHeight);
	return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppIterate(void* appstate) {
	switch (screen) {  //определяет, какой "экран" рисовать
	case MAIN_MENU:
		drawMainMenu();
		break;
	case GAME_SCREEN:
		drawFrame(map->tileMap, Z); //!!!
		break;
	};


	SDL_RenderPresent(renderer); //закончить прорисовку
	return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) { //ивенты
	switch (event->type) {
	case SDL_EVENT_QUIT: //выход из приложения
		return SDL_APP_SUCCESS;
		break;
	case SDL_EVENT_KEY_DOWN ://клавиатура
		SDL_Keycode key = event->key.key;
		if (key == SDLK_R) {
			if (Z < 256) {
				Z++;
			}
		}
		else if (key == SDLK_F) {
			if (Z > 0) {
				Z--;
			}
		}
		break;
	}
	return SDL_APP_CONTINUE;
};

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
};