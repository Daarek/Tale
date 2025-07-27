#pragma once
#include "templates.h"
#include "localMap.h"
#include "projectData.h"

arr2d<float, 256, 256>* createHeightMap(int order, int startScale, int seed);

int generateSeed();

void generators_getData(Data* d);
void firstGeneratingSequence();
/*
float* createGrid(int seed, int size);
*/
float getNoiseValue(float x, float y, float* vectors, int size);
void createChunk(int globalX, int globalY);