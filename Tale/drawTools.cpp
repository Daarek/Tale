#include "SDL.h"
#include "enums.h"

static SDL_Renderer* renderer = NULL;//renderer

static SDL_Texture* placeholder = NULL;//�������� ��������
static SDL_Texture* betaTileset = NULL;//�������� �������     ===> �������� ��� �� 32�32 ����� ������
static SDL_Texture* rat16 = NULL; //���

static SDL_FRect cut(int x) { //�������, ������� ������ ��������� �������� �� ��� ����������� �� ������
	return { (float)(16 * x), 0.f, 16.f, 16.f };
};

void drawToolsInit(SDL_Renderer* rend) { //�������, ����� �������� �� ������ �� main ����
	renderer = rend;
	
	placeholder = IMG_LoadTexture(renderer, "Assets/coconut.png"); //������������� ��������
	betaTileset = IMG_LoadTexture(renderer, "Assets/ts_simple.png");
	rat16 = IMG_LoadTexture(renderer, "Assets/rat.png");
};

void drawTile(Tile tile) { //���������� ����
	SDL_FRect i;

	switch (tile) { //�������� ���� ��� ������� � ������� ��� � ��������
	case GRASS:
		i = cut(1);
		break;
	case STONE:
		i = cut(2);
		break;

	}
	SDL_RenderTexture(renderer, betaTileset, &i, NULL); //������

	SDL_RenderPresent(renderer); //������� �����
};


