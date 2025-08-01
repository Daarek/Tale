#pragma once
#include <unordered_map>
#include <string>
#include "enums.h"
typedef void(*voidFunction)();
struct Button {
	int x;
	int y;
	int width;
	int height;
	voidFunction action;
	bool isInbound(int touchX, int touchY) {
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