#include "enums.h"
#include "templates.h"
#include "SDL.h"
#include "generators.h"
#include "temporary.h"
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
	arr3d<Tile, 64, 64, 256>* result = new arr3d<Tile, 64, 64, 256>{};//3d карта на выходе
	arr2d<float, 256, 256>* height = createHeightMap(1, 2, seed);//карта высот
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			int h = (floor((*height)[x*4][y*4] * 10) + 128);//высота в этом месте, *4 так как высотная карта в 4 раза больше
			for (int z = 0; z < h; z++) {
				(*result)[z][x][y] = STONE;//заполняю
			}
			(*result)[h][x][y] = GRASS;//верхний слой травы
		}
	}
	delete(height);//почистить
	return result;
}
/*

*/