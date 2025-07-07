#include "enums.h"
#include "templates.h"
#include "SDL.h"
#include "generators.h"
#include <iostream>
arr3d<Tile, 64, 64, 256>* generateRandom(){
	arr3d<Tile, 64, 64, 256>* result = new arr3d<Tile, 64, 64, 256>;
	for (int z = 0; z < 256; z++) {
		for (int x = 0; x < 64; x++) {
			for (int y = 0; y < 64; y++) {
				int r = SDL_rand(2);
				if (r == 0) {
					(*result)[z][x][y] = GRASS;
				}
				else {
					(*result)[z][x][y] = STONE;
				}
			}
		}
	}

	return result;
};

arr3d<Tile, 64, 64, 256>* generateOnHeightMap(int seed) {
	arr3d<Tile, 64, 64, 256>* result = new arr3d<Tile, 64, 64, 256>{};
	arr2d<float, 64, 64>* height = createHeightMap(1, 2, seed);
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			int h = (floor((*height)[x][y] * 10) + 128);//высота в этом месте
			for (int z = 0; z < h; z++) {
				(*result)[z][x][y] = STONE;//заполняю
			}
			(*result)[h][x][y] = GRASS;//верхний слой травы
		}
	}
	delete(height);//почистить
	return result;
}