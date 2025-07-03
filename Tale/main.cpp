#include "SDL.h"
#define	SDL_MAIN_USE_CALLBACKS
#include "SDL3/SDL_main.h"
#include "enums.h"
#include "drawTools.h"
#include "localMap.h"
#include "temporary.h"
#include <string>
#include <iostream>
#include "inputHandler.h"
#include "generators.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
static LocalMap* map = new LocalMap; //локальная карта, переместить потом
static Screen screen = MAIN_MENU; //текущий экран
static int screenWidth = 1920;
static int screenHeight = 1080;
static int seed;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
	seed = generateSeed();
	map->tileMap = generateOnHeightMap(seed);//!!!
	map->viewedZLevel = 128;
	inputHandlerInit(map, screenWidth, screenHeight); //инициализация инпут функций
	SDL_Init(SDL_INIT_VIDEO); //инициализация всего
	window = SDL_CreateWindow("main", screenWidth, screenHeight, NULL);
	renderer = SDL_CreateRenderer(window, NULL);
	drawToolsInit(renderer, screenWidth, screenHeight);//запихиваю важные переменные в drawTools

	return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppIterate(void* appstate) {
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	switch (screen) {  //определяет, какой "экран" рисовать
	case MAIN_MENU:
		drawMainMenu();
		break;
	case GAME_SCREEN:
		drawFrame(map->tileMap, map->viewedZLevel); //!!!
		break;
	};
	
	SDL_RenderPresent(renderer); //закончить прорисовку
	return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) { //ивенты
	switch (event->type) {

	case SDL_EVENT_QUIT: {//выход из приложения
		return SDL_APP_SUCCESS;
		break;
	}

	case SDL_EVENT_KEY_DOWN: {//клавиатура
		keyboardInput(event, screen);
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_DOWN: {//жмак мышкой 
		mouseInput(event, screen);
		break;
	}

	}
	return SDL_APP_CONTINUE;
};

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	delete(map);
};