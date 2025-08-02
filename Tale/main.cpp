#include "SDL.h"
#include <string>
#define	SDL_MAIN_USE_CALLBACKS
#include "SDL3/SDL_main.h"
#include "enums.h"
#include "drawTools.h"
#include "localMap.h"
#include <string>
#include <iostream>
#include "inputHandler.h"
#include "generators.h"
#include "saveFileHandler.h"
#include "projectData.h"

SDL_Window* window = NULL;
static Data* data = new Data; //хранилище всей необходимой для работы информации

void initButton(Screen scr, std::string name, int x, int y, int w, int h, voidFunction act) {//автоматически создаёт кнопку из аргументов
	data->menus->menus[scr].buttons[name] = {};
	data->menus->menus[scr].buttons[name].x = x;
	data->menus->menus[scr].buttons[name].y = y;
	data->menus->menus[scr].buttons[name].width = w;
	data->menus->menus[scr].buttons[name].height = h;
	data->menus->menus[scr].buttons[name].action = act;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
	data->globalMap = new GlobalMap;
	data->globalMap->viewedZLevel = 128;
	data->globalMap->octaveAmount = 4;//уровень детализации
	data->globalMap->initSize = 4;//грубо говоря, размер биомов (количество чанков шума)
	data->globalMap->globalMapSideSize = 64;
	data->scale = 64;
	data->windowHeight = 1080;
	data->windowWidth = 1920;
	data->side = (int)(data->windowHeight / data->scale);
	data->startOffsetY = (int)((data->windowHeight - ((int)(data->windowHeight / data->scale) * data->scale)) / 2);
	data->startOffsetX = (int)((data->windowWidth - ((int)(data->windowHeight / data->scale) * data->scale)) / 2);
	data->screen = MAIN_MENU;
	data->menus = new Menus;
	data->menus->menus[MAIN_MENU] = {}; //создаю меню главное меню
	data->menus->menus[GAME_SCREEN_GLOBAL_MAP] = {};
	initButton(MAIN_MENU, "start", (0.45 * data->windowWidth), (0.45 * data->windowHeight), (0.1 * data->windowWidth), (0.03 * data->windowWidth), []() {data->globalMap->seed = generateSeed(); firstGeneratingSequence(); data->screen = GAME_SCREEN_GLOBAL_MAP;});
	initButton(GAME_SCREEN_GLOBAL_MAP, "map", data->startOffsetX, data->startOffsetY, (data->windowWidth - 2*data->startOffsetX), (data->windowHeight - 2*data->startOffsetY), [](){createChunk(); data->screen = GAME_SCREEN;});
	arr3d<Tile, 64, 64, 256>* temp = new arr3d<Tile, 64, 64, 256>;
	data->globalMap->tileMap = temp;
	inputHandler_getData(data); //инициализация инпут функций
	SDL_Init(SDL_INIT_VIDEO); //инициализация всего
	window = SDL_CreateWindow("main", data->windowWidth, data->windowHeight, NULL);
	data->renderer = SDL_CreateRenderer(window, NULL);
	drawTools_getData(data);
	generators_getData(data);
	return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppIterate(void* appstate) {
	
	SDL_SetRenderDrawColor(data->renderer, 0, 0, 0, 255);
	SDL_RenderClear(data->renderer);
	switch (data->screen) {  //определяет, какой "экран" рисовать
	case MAIN_MENU:
		drawMainMenu();
		break;
	case GAME_SCREEN:
		drawFrame();
		break;
	case GAME_SCREEN_GLOBAL_MAP:
		drawMap();
		break;
	};
	
	SDL_RenderPresent(data->renderer); //закончить прорисовку
	return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) { //ивенты
	switch (event->type) {

	case SDL_EVENT_QUIT: {//выход из приложения
		save(data->globalMap->seed);
		return SDL_APP_SUCCESS;
		break;
	}

	case SDL_EVENT_KEY_DOWN: {//клавиатура
		keyboardInput(event);
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_DOWN: {//жмак мышкой 
		mouseInput(event);
		break;
	}

	}
	return SDL_APP_CONTINUE;
};

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(data->renderer);
	data->renderer = NULL;
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	delete[](data->globalMap->tileMap);
	delete[](data->globalMap->globalMap);
	delete[](data->globalMap->perlinGrid);
	delete(data->globalMap);
	delete(data->menus);
	delete(data);
};