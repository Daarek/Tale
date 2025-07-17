#pragma once
#include "SDL.h"
#include "enums.h"

void mouseInput(SDL_Event* event, Screen& screen);
void keyboardInput(SDL_Event* event, Screen& screen);
void inputHandlerInit(GlobalMap* m, int w, int h);