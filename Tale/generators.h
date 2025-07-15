#pragma once
#include "templates.h"

arr2d<float, 256, 256>* createHeightMap(int order, int startScale, int seed);

int generateSeed();
/*
float* createGrid(int seed, int size);
float getNoiseValue(double x, double y, float* vectors, int size);
*/