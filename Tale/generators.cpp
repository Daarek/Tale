#include "templates.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>
#include "localMap.h"
#include "enums.h"
//using namespace std;
static float M_PI = 3.14159265;
static float rad = (M_PI / 180);

GlobalMap* map;

int generateSeed() {

    srand(time(0));

    return rand();

}

void generatorsInit(GlobalMap* m) {
    map = m;
}

static float smoothStep(float t) {
    return t * t * (3 - 2 * t);
}

static float interpolate(float x1, float y1, float x2, float y2, float x3, float y3, float weight1, float weight2) {
    //����� �������� ���� ����� 2�� �������
    float a1 = x3 - x1;
    float a2 = x3 - x2;
    float b1 = y3 - y1;
    float b2 = y3 - y2;
    float d1 = pow((pow(a1, 2) + pow(b1, 2)), 0.5);
    float d2 = pow((pow(a2, 2) + pow(b2, 2)), 0.5);
    float amp = abs(weight1 - weight2);
    float relx;
    float grand;
    if (weight1 > weight2) {
        grand = weight2;
        relx = d2 / (d1 + d2);
    }
    else {
        grand = weight1;
        relx = d1 / (d1 + d2);
    }
    return amp * smoothStep(relx) + grand;
}

float getNoiseValue(float x, float y, float* vectors, int size) {//����� �������� ���� � ����� �� ����� �������
    int xl = floor(x); //x ���� ����� ��������
    int yb = floor(y); //y ���� ������ ��������

    if ((xl != x) and (yb != y)) { //���� ����� �� ��������� ����� ��������
        int yt = yb + 1; //y ���� ������� ��������
        int xr = xl + 1; //x ���� ������ ��������

        float vbl = vectors[xl + (size + 1) * yb]; //������ ������� ������ ����
        float vtl = vectors[xl + (size + 1) * yt]; //������ �������� ������ ����
        float vtr = vectors[xr + (size + 1) * yt]; //������ �������� ������� ����
        float vbr = vectors[xr + (size + 1) * yb]; //������ ������� ������� ����

        float pbl = (cos(vbl * rad) * (x - xl)) + (sin(vbl * rad) * (y - yb)); //��������� ������������, �����
        float pbr = (cos(vbr * rad) * (x - xr)) + (sin(vbr * rad) * (y - yb));
        float ptl = (cos(vtl * rad) * (x - xl)) + (sin(vtl * rad) * (y - yt));
        float ptr = (cos(vtr * rad) * (x - xr)) + (sin(vtr * rad) * (y - yt));

        float pt = interpolate(xl, yt, xr, yt, x, yt, ptl, ptr); //����� ����� ��� �������
        float pb = interpolate(xl, yb, xr, yb, x, yb, pbl, pbr); //����� ����� ��� �������
        return interpolate(x, yt, x, yb, x, y, pt, pb);
        
    }
    else if ((xl == x) and (yb != y)) { //����� ��������� �� ������������ �����������
        int yt = yb + 1;

        float vbl = vectors[xl + (size + 1) * yb]; //������ ������� ����
        float vtl = vectors[xl + (size + 1) * yt]; //������ �������� ����
       
        float pb = (cos(vbl * rad) * (x - xl)) + (sin(vbl * rad) * (y - yb)); //��������� ������������, �����
        float pt = (cos(vtl * rad) * (x - xl)) + (sin(vtl * rad) * (y - yt));

        return interpolate(xl, yt, xl, yb, xl, y, pt, pb);
    }
    else if (((xl != x) and (yb == y))) { //����� ��������� �� �������������� �����������
        int xr = xl + 1;

        float vbl = vectors[xl + (size + 1) * yb]; //������ ������ ����
        float vbr = vectors[xr + (size + 1) * yb]; //������ ������� ����

        float pl = (cos(vbl * rad) * (x - xl)) + (sin(vbl * rad) * (y - yb)); //��������� ������������, �����
        float pr = (cos(vbr * rad) * (x - xr)) + (sin(vbr * rad) * (y - yb));

        return interpolate(xl, yb, xr, yb, x, yb, pl, pr);
    }

    return 0; //����� �� ����
}

float* createGrid(int seed, int size) {//size - ���������� ������
    int lenght = (size + 1) * (size + 1);
    float* vectors = new float[lenght]; //�� ������ ���������
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(1, 3600);
    for (int i = 0; i < lenght; i++) {
        vectors[i] = dist(rng) / 10;

    }
    return vectors;
}

void firstGeneratingSequence() {

    int perlinArrayLenght = 0;

    for (int i = 0; i < map->octaveAmount; i++) {
        perlinArrayLenght += (int)pow((pow(2, i) * map->initSize + 1), 2);
    }

    float* perlinArray = new float[perlinArrayLenght];//������, ���� � ������ ��� �������
    int slider = 0;
    for (int octave = 1; octave <= map->octaveAmount; octave++) { //��������� ��� ������ � ������ ���� �� ������
        float* currentOctave = new float[(int)pow((pow(2, octave-1) * map->initSize + 1), 2)]; //������� ������ (���� ������)
        currentOctave = createGrid(map->seed, (int)(pow(2, octave-1)*map->initSize)); //�������� ������
        for (int i = 0; i < (int)pow((pow(2, octave - 1) * map->initSize + 1), 2); i++) { //�������� ������ � ������
            perlinArray[slider] = currentOctave[i];
            slider++;
        }
        delete[](currentOctave);
    }

    map->perlinGrid = perlinArray; //�������� ���

    int size = (int)(pow(map->globalMapSideSize, 2));
    GlobalTile* globalMapArray = new GlobalTile[size]; //���� ��������� ���������� �����

    //������ �������� ���������� �����
    int t = 0;
    for (int i = 0; i < map->octaveAmount; i++) {
        t = t + pow(2, i);
    }
    int low = (t / pow(2, (map->octaveAmount - 1))); //�����, �� ������� ����� �������� ��� ������ ����� �������� ������ ����� �� ����� ������

    //int heightMapSize = (int)(pow(map->globalMapSideSize, 2));
    float* heightMap = new float[size];//����� �����
    for (int i = 0; i < size; i++) {//�������
        heightMap[i] = 0.0f;
    }

    for (int octave = 1; octave <= map->octaveAmount; octave++) { //��������� ����� �� ������

        int l = (int)(pow(2, octave - 1) * map->initSize + 1);//������ ������� ����� ������
        float* stackGrid = new float[(int)(pow(l, 2))]; //������� ������
        for (int i = 0; i < (int)(pow(l, 2)); i++) {//�������� ������
            stackGrid[i] = map->perlinGrid[map->getOctaveOffset(octave) + i];
        }

        float step = pow(map->initSize, octave) / map->globalMapSideSize; //������� ����������� �� ������ ������������� ���������� �� arr2d

        for (int x = 0; x < map->globalMapSideSize; x++) {//������ �������� �����
            for (int y = 0; y < map->globalMapSideSize; y++) {
                heightMap[x + y * map->globalMapSideSize] += getNoiseValue(x*step, y*step, stackGrid, l)/pow(2, octave - 1);
            }
        }

        delete[](stackGrid);//������� �����
    }

    for (int i = 0; i < (int)pow(map->globalMapSideSize, 2); i++) {//����������� � ������ � �����
        heightMap[i] = (heightMap[i]/low * 49 + 128);
    }

    for (int i = 0; i < (int)pow(map->globalMapSideSize, 2); i++) {//����������� � globalMap �����
        if (heightMap[i] <= 118) {
            globalMapArray[i] = LOWLANDS;
        }
        else if (heightMap[i] >= 138) {
            globalMapArray[i] = MOUNTAINS;
        }
        else {
            globalMapArray[i] = PLAINS;
        }
    }
    map->globalMap = globalMapArray;//�������� ������ � �������
    delete[](heightMap);//������ ����� �����
    
}

arr2d<float, 256, 256>* createHeightMap(int order, int startScale, int seed) {//order - ���������� �����, startScale - ����� ������� �� ������ ������
    if ((startScale * pow(2, order)) > 256) { //����� ������ ���� ���� �� ����� ���� ����� ��������������� ���� �����
        std::cout << "error";
        return nullptr;
    }
    int t = 0;
    for (int i = 0; i < order; i++) {
        t = t + pow(2, i);
    }
    int low = (t / pow(2, (order - 1))); //�����, �� ������� ����� �������� ��� ������ ����� �������� ������ ����� �� ����� ������
    arr2d<float, 256, 256>* heightMap = new arr2d<float, 256, 256>{}; //��������� �� ����� �����

    for (int i = 1; i <= order; i++) { //��������� ����� �� ������

        float* stack_grid = createGrid(seed, pow(startScale, i));//������� ������
        float step = pow(startScale, i) / 256; //������� ����������� �� ������ ������������� ���������� �� arr2d

        for (int x = 0; x < 256; x++) {
            for (int y = 0; y < 256; y++) {
                (*heightMap)[x][y] += (getNoiseValue(x * step, y * step, stack_grid, pow(startScale, i)) /pow(2, i - 1)); //���� �������� �� 1
            }
         }

        delete[](stack_grid);//��������� ������
    }
    
    for (int x = 0; x < 256; x++) { // �����
        for (int y = 0; y < 256; y++) {
            (*heightMap)[x][y] /= low;
        }
    }
    
    return heightMap;

}
