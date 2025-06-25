#include "enums.h"
#include "templates.h"
#include "SDL.h"

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