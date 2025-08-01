#include "localMap.h"
#include "menus.h"
#include "enums.h"
#include "SDL.h"

struct Data {
	GlobalMap* globalMap;//структ текущего мира
	SDL_Renderer* renderer;
	int windowWidth; //ширина и высота монитара
	int windowHeight;
	int scale; //сколько тайлов должно помещаться на экране
	int side;//длинна стороны тайла в пикселях
	int startOffsetX; //координата X и Y самого левого верхнего угла самого верхнего левого тайла
	int startOffsetY;
	Screen screen;
	Menus menus;//все меню.
};