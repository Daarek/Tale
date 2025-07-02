#include "SDL.h"
#include "enums.h"
#include "templates.h"

static SDL_Renderer* renderer = NULL;//renderer

static SDL_Texture* placeholder = NULL;//�������� �����������
static SDL_Texture* betaTileset = NULL;//�������� �������     ===> �������� ��� �� 32�32 ����� ������
static SDL_Texture* rat16 = NULL; //���

static int scale = 64; //� ������� ��� ������� ���������������, ������� ������ ���������� �� �����
static int screenWidth;
static int screenHeight;

static SDL_FRect cut(int x, int y) { //�������, ������� ������ ��������� �������� �� ��� ����������� �� ������
	return { (float)(32 * x), (float)(32 * y), 32.f, 32.f };
};

void drawToolsInit(SDL_Renderer* rend, int w, int h) { //�������, ����� �������� �� ������ �� main ����
	renderer = rend;
	
	placeholder = IMG_LoadTexture(renderer, "Assets/coconut.png"); //������������� ��������
	betaTileset = IMG_LoadTexture(renderer, "Assets/ts_simple32.png");
	rat16 = IMG_LoadTexture(renderer, "Assets/rat.png");

	screenWidth = w;
	screenHeight = h;
};

void drawTile(Tile tile, int x, int y) { //���������� ����
	SDL_FRect i; //����� ���� ��������
	int side = (int)(1024 / scale); //������ ������� ����� � ��������
	SDL_FRect pos = { x*side, y*side, side, side }; //�����, ���� �� ������ ���������� ����
	switch (tile) { //�������� ���� ��� ������� � ������� ��� � ��������
	case AIR:
		break;
	case GRASS:
		i = cut(0, 0);
		break;
	case STONE: //�������� �����, �� �����
		i = cut(1, 0);
		break;

	};
	SDL_RenderTexture(renderer, betaTileset, &i, &pos); //������
};

void drawFrame(const arr3d<Tile, 64, 64, 256>* arr, int z) { //����� �� ����������, ��� ����, ������ GAME_SCREEN
	for (int x = 0; x < 64; x++) {
		for (int y = 0; y < 64; y++) {
			drawTile((*arr)[z][x][y], x, y);
		};
	};
};

void drawMainMenu() { //������ ������� ����

	//������ ������ �����, ���������� ����� � ����������
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 - screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	SDL_RenderLine(renderer, (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 - screenHeight / 20), (screenWidth / 2 + screenWidth / 20), (screenHeight / 2 + screenHeight / 20));
	
};