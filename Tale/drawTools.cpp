#include "SDL.h"
#include "enums.h"


static SDL_Renderer* renderer = NULL;//renderer

static SDL_Texture* placeholder = NULL;//�������� ��������
static SDL_Texture* betaTileset = NULL;//�������� �������     ===> �������� ��� �� 32�32 ����� ������
static SDL_Texture* rat16 = NULL; //���

static int scale = 64; //� ������� ��� ������� ���������������

static SDL_FRect cut(int x, int y) { //�������, ������� ������ ��������� �������� �� ��� ����������� �� ������
	return { (float)(16 * x), (float)(16 * y), 16.f, 16.f };
};

void drawToolsInit(SDL_Renderer* rend) { //�������, ����� �������� �� ������ �� main ����
	renderer = rend;
	
	placeholder = IMG_LoadTexture(renderer, "Assets/coconut.png"); //������������� ��������
	betaTileset = IMG_LoadTexture(renderer, "Assets/ts_simple.png");
	rat16 = IMG_LoadTexture(renderer, "Assets/rat.png");
};

void drawTile(Tile tile, int x, int y) { //���������� ����
	SDL_FRect i; //����� ���� ��������
	int side = (int)(1024 / scale); //������ ������� ����� � ��������
	SDL_FRect pos = { x*side, y*side, side, side }; //�����, ���� �� ������ ���������� ����
	switch (tile) { //�������� ���� ��� ������� � ������� ��� � ��������
	case GRASS:
		i = cut(1, 0);
		break;
	case STONE:
		i = cut(2, 0);
		break;

	};
	SDL_RenderTexture(renderer, betaTileset, &i, &pos); //������
};

void drawFrame() { //����� �� ����������, ��� ����
	for (int x = 0; x < scale; x++) {
		for (int y = 0; y < scale; y++) {
			int r = SDL_rand(2);
			if (r == 1) {
				drawTile(GRASS, x, y);
			}
			else {
				drawTile(STONE, x, y);
			}
		};
	};
}
