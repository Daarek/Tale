

#include "SDL.h"
#define	SDL_MAIN_USE_CALLBACKS
#include "SDL3/SDL_main.h"
#include "enums.h"
#include "drawTools.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;



SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("main", 1920, 1080, NULL);
	renderer = SDL_CreateRenderer(window, NULL);
	drawToolsInit(renderer);
	return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppIterate(void* appstate) {
	drawTile(STONE);
	SDL_Delay(2000);
	return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	switch (event->type) {
	case SDL_EVENT_QUIT:
		return SDL_APP_SUCCESS;
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