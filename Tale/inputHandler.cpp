#include "SDL.h"
#include "enums.h"
#include "localMap.h"

static int windowWidth;
static int windowHeight;
static LocalMap* map;

void inputHandlerInit(LocalMap* m, int w, int h) {
	map = m;
	windowWidth = w;
	windowHeight = h;
}

void mouseInput(SDL_Event* event, Screen &screen) {
	int x = event->button.x;
	int y = event->button.y;

	//������� ������ ����� ������� ����� �� ����� ����
	if ((x > 0.45 * windowWidth) and (x < 0.55 * windowWidth) and (y > 0.45 * windowHeight) and (y < 0.55 * windowHeight)) {
		screen = GAME_SCREEN;
	}
}

void keyboardInput(SDL_Event* event, Screen &screen) {
	switch (screen) {

	case GAME_SCREEN: {
		SDL_Keycode key = event->key.key;
		if (key == SDLK_R) { //���������� �� ������� �����
			map->viewedZLevel++;
		}
		else if (key == SDLK_F) {// ���������� �� ������� ����
			map->viewedZLevel--;
		}
		break;
	}

	case MAIN_MENU: {
		break;
	}

	}
}