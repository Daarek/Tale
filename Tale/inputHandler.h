#pragma once
#include "SDL.h"
#include "enums.h"
#include "projectData.h"

void mouseInput(SDL_Event* event);
void keyboardInput(SDL_Event* event);
void inputHandler_getData(Data* d);