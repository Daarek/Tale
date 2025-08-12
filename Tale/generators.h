#pragma once
#include "templates.h"
#include "localMap.h"
#include "projectData.h"

int generateSeed();

void generators_getData(Data* d);
void firstGeneratingSequence();
float getNoiseValue(float x, float y, float* vectors, int size);
void createChunk();
void createRegionalChunk();
void createSubChunk();