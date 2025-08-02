#include <unordered_map>
#include "SDL.h"
#include <string>
#include "enums.h"
typedef void(*voidFunction)();
struct Button {
	int x;//координаты угла кнопки
	int y;
	int width; //ширина высота кнопки
	int height;
	SDL_Texture* texture;
	voidFunction action; //что кнопка делает
	bool isInbound(int touchX, int touchY) { //проверить, если клик на кнопке
		if ((touchX >= x) and (touchY >= y) and (touchX <= (x + width)) and (touchY <= (y + height))) {
			return true;
		}
		return false;
	}
};
typedef std::unordered_map<std::string, Button> buttonMap;
struct Menu {
	buttonMap buttons;
};
typedef std::unordered_map<Screen, Menu> menuMap;
struct Menus {
	menuMap menus;
};