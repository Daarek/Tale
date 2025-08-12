#include "templates.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <random>
#include "localMap.h"
#include "enums.h"
#include "projectData.h"
static float M_PI = 3.14159265;
static float rad = (M_PI / 180);

static Data* data;

int generateSeed() {

    srand(time(0));

    return rand();

}

void generators_getData(Data* d) {
    data = d;
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

float getNoiseValue(float x, float y, float* vectors, int size) {//����� �������� ���� � ����� �� ����� �������, size � �����������
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

    for (int i = 0; i < data->globalMap->octaveAmount; i++) {
        perlinArrayLenght += (int)pow((pow(2, i) * data->globalMap->initSize + 1), 2);
    }

    float* perlinArray = new float[perlinArrayLenght];//������, ���� � ������ ��� �������
    int slider = 0;
    for (int octave = 1; octave <= data->globalMap->octaveAmount; octave++) { //��������� ��� ������ � ������ ���� �� ������
        float* currentOctave = new float[(int)pow((pow(2, octave-1) * data->globalMap->initSize + 1), 2)]; //������� ������ (���� ������)
        currentOctave = createGrid(data->globalMap->seed, (int)(pow(2, octave-1)*data->globalMap->initSize)); //�������� ������
        for (int i = 0; i < (int)pow((pow(2, octave - 1) * data->globalMap->initSize + 1), 2); i++) { //�������� ������ � ������
            perlinArray[slider] = currentOctave[i];
            slider++;
        }
        delete[](currentOctave);
    }

    data->globalMap->perlinGrid = perlinArray; //�������� ���

    int size = (int)(pow(data->globalMap->globalMapSideSize, 2));//������ ����� � ���-�� ������
    GlobalTile* globalMapArray = new GlobalTile[size]; //���� ��������� ���������� �����

    //������ �������� ���������� �����
    int t = 0;
    for (int i = 0; i < data->globalMap->octaveAmount; i++) {
        t = t + pow(2, i);
    }
    int low = (t / pow(2, (data->globalMap->octaveAmount - 1))); //�����, �� ������� ����� �������� ��� ������ ����� �������� ������ ����� �� ����� ������

    float* heightMap = new float[size];//����� �����
    for (int i = 0; i < size; i++) {//�������
        heightMap[i] = 0.0f;
    }
    for (int octave = 1; octave <= data->globalMap->octaveAmount; octave++) { //��������� ����� �� ������

        int l = (int)(pow(2, octave - 1) * data->globalMap->initSize + 1);//������ ������� ����� ������
        float* stackGrid = new float[(int)(pow(l, 2))]; //������� ������
        for (int i = 0; i < (int)(pow(l, 2)); i++) {//�������� ������
            stackGrid[i] = data->globalMap->perlinGrid[data->globalMap->getOctaveOffset(octave - 1) + i];
        }

        float step = data->globalMap->initSize * pow(2, octave - 1) / data->globalMap->globalMapSideSize; //������� ����������� �� ������ ������������� ���������� �� arr2d
        float mStep = step * 0.5;
        for (int x = 0; x < data->globalMap->globalMapSideSize; x++) {//������ �������� �����
            for (int y = 0; y < data->globalMap->globalMapSideSize; y++) {
                heightMap[x + y * data->globalMap->globalMapSideSize] += getNoiseValue(x * step + mStep, y * step + mStep, stackGrid, l) / pow(2, octave - 1);
            }
        }

        delete[](stackGrid);//������� �����
    }

    for (int i = 0; i < (int)pow(data->globalMap->globalMapSideSize, 2); i++) {//����������� � ������ � �����
        heightMap[i] = (int)(heightMap[i]/low * 49 + 128);
    }

    for (int i = 0; i < (int)pow(data->globalMap->globalMapSideSize, 2); i++) {//����������� � globalMap �����
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
    data->globalMap->globalMap = globalMapArray;//�������� ������ � �������
    delete[](heightMap);//������ ����� �����
    
}

void createChunk() {
    int t = 0;
    for (int i = 0; i < data->globalMap->octaveAmount; i++) {
        t = t + pow(2, i);
    }
    int low = (t / pow(2, (data->globalMap->octaveAmount - 1))); //�����, �� ������� ����� �������� ��� ������ ����� �������� ������ ����� �� ����� ������

    float* heightMap = new float[4096];//����� ����� ��� 1 ���������� �����, 64�64 = 4096
    for (int i = 0; i < 4096; i++) {//�������
        heightMap[i] = 0.0f;
    }

    for (int octave = 1; octave <= data->globalMap->octaveAmount; octave++) { //��������� ����� �� ������

        int l = (int)(pow(2, octave - 1) * data->globalMap->initSize + 1);//������ ������� ����� ������
        float* stackGrid = new float[(int)(pow(l, 2))]; //������� ������
        for (int i = 0; i < (int)(pow(l, 2)); i++) {//�������� ������
            stackGrid[i] = data->globalMap->perlinGrid[data->globalMap->getOctaveOffset(octave - 1) + i];
        }

        float step = data->globalMap->initSize * pow(2, octave - 1) / data->globalMap->globalMapSideSize; //������� ����������� �� ������ ������������� ���������� �� arr2d
        float mStep = step / data->globalMap->regionalMapSideSize; ////������� ����������� �� ������ ������������� ���������� �� ��� �����
        float mmStep = mStep / 64; //������� ����������� �� ������ ������������� ���������� �� �����
        for (int mx = 0; mx < 64; mx++) {//������ �������� �����, mx � my - ���������� ������ � �����
            for (int my = 0; my < 64; my++) {
                heightMap[mx + my * 64] += getNoiseValue(data->globalMap->viewedChunkX * step + data->globalMap->viewedRegChunkX * mStep + mx * mmStep, data->globalMap->viewedChunkY * step + data->globalMap->viewedRegChunkY * mStep + my * mmStep, stackGrid, l) / pow(2, octave - 1);
            }
        }//���� ������ - ������������ ��� ���������� (��������)

        delete[](stackGrid);
    }

    for (int i = 0; i < (int)pow(data->globalMap->globalMapSideSize, 2); i++) {//����������� � ������ � �����
        heightMap[i] = (int)(heightMap[i] / low * 49 + 128);

    }

    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 64; y++) {
            for (int z = 0; z < 256; z++) {
                if (z < heightMap[x + 64 * y]) {
                    (*data->globalMap->tileMap)[z][x][y] = STONE;
                }
                else if (z == heightMap[x + 64 * y]) {
                    (*data->globalMap->tileMap)[z][x][y] = GRASS;
                }
                else {
                    (*data->globalMap->tileMap)[z][x][y] = AIR;
                }
            }
        }
    }
    delete[](heightMap);
}


void createRegionalChunk() {
    int t = 0;
    for (int i = 0; i < data->globalMap->octaveAmount; i++) {
        t = t + pow(2, i);
    }
    int low = (t / pow(2, (data->globalMap->octaveAmount - 1))); //�����, �� ������� ����� �������� ��� ������ ����� �������� ������ ����� �� ����� ������

    float* heightMap = new float[(int)pow(data->globalMap->regionalMapSideSize, 2)];//����� ����� ��� 1 ���. �����
    for (int i = 0; i < (int)pow(data->globalMap->regionalMapSideSize, 2); i++) {//�������
        heightMap[i] = 0.0f;
    }

    for (int octave = 1; octave <= data->globalMap->octaveAmount; octave++) { //��������� ����� �� ������

        int l = (int)(pow(2, octave - 1) * data->globalMap->initSize + 1);//������ ������� ����� ������
        float* stackGrid = new float[(int)(pow(l, 2))]; //������� ������
        for (int i = 0; i < (int)(pow(l, 2)); i++) {//�������� ������
            stackGrid[i] = data->globalMap->perlinGrid[data->globalMap->getOctaveOffset(octave - 1) + i];
        }

        float step = data->globalMap->initSize * pow(2, octave - 1) / data->globalMap->globalMapSideSize; //������� ����������� �� ������ ������������� ���������� �� arr2d
        float mStep = step / data->globalMap->regionalMapSideSize; ////������� ����������� �� ������ ������������� ���������� �� �����
        //float mmStep = mStep / 16; //������� ����������� �� ������ ������������� ���������� �� ��������
        for (int mx = 0; mx < data->globalMap->regionalMapSideSize; mx++) {//������ �������� �����, mx � my - ���������� ������ � �����
            for (int my = 0; my < data->globalMap->regionalMapSideSize; my++) {
                heightMap[mx + my * data->globalMap->regionalMapSideSize] += getNoiseValue(data->globalMap->viewedChunkX * step + mx * mStep, data->globalMap->viewedChunkY * step + my * mStep, stackGrid, l) / pow(2, octave - 1);
            }
        }//���� ������ - ������������ ��� ���������� (��������)

        delete[](stackGrid);
    }

    for (int i = 0; i < (int)pow(data->globalMap->regionalMapSideSize, 2); i++) {//����������� � ������ � ����
        heightMap[i] = (int)(heightMap[i] / low * 49 + 128);

    }

    GlobalTile* regionalMapArray = new GlobalTile[(int)pow(data->globalMap->regionalMapSideSize, 2)];//������� �����
    for (int x = 0; x < data->globalMap->regionalMapSideSize; x++) {
        for (int y = 0; y < data->globalMap->regionalMapSideSize; y++) {
            if (heightMap[x + data->globalMap->regionalMapSideSize * y] >= 138) {
                regionalMapArray[x + data->globalMap->regionalMapSideSize * y] = MOUNTAINS;
            }
            else if (heightMap[x + data->globalMap->regionalMapSideSize * y] <= 118) {
                regionalMapArray[x + data->globalMap->regionalMapSideSize * y] = LOWLANDS;
            }
            else {
                regionalMapArray[x + data->globalMap->regionalMapSideSize * y] = PLAINS;
            }
        }
    }
    data->globalMap->regionalMap = regionalMapArray;
    delete[](heightMap);
}

void createSubChunk() {

}