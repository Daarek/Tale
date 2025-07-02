#include "templates.h"
#include <iostream>
//#include<cstdlib>
#include<ctime>
#include<cmath>
//using namespace std;
static float M_PI = 3.14159265;
static float rad = (M_PI / 180);
int generateSeed() {

    srand(time(0));

    return rand();

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
    float relx = d1 / (d1 + d2);
    float grand;
    if (weight1 > weight2) {
        grand = weight2;
    }
    else {
        grand = weight1;
    }
    return amp * smoothStep(relx) + grand;
}

float getNoiseValue(double x, double y, float* vectors, int size) {//����� �������� ���� � ����� �� ����� �������
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
    float* vectors = new float[lenght]; //�� ������ ��������
    srand(seed);
    for (int i = 0; i < lenght; i++) {
        vectors[i] = rand() % 361;
    }
    return vectors;
}

arr2d<float, 64, 64>* createHeightMap(int order, int startScale, int seed) {//order - ���������� �����, startScale - ����� ������� �� ������ ������
    if ((startScale * pow(2, order)) > 64) { //����� ������ ���� ���� �� ����� ���� ����� ��������������� ���� �����
        std::cout << "error";
        return nullptr;
    }
    srand(seed);
    int t = 0;
    for (int i = 0; i < order; i++) {
        t = t + pow(2, i);
    }
    int low = (t / pow(2, (order - 1))); //�����, �� ������� ����� �������� ��� ������ ����� �������� ������ ����� �� ����� ������
    arr2d<float, 64, 64>* heightMap = new arr2d<float, 64, 64>{}; //��������� �� ����� �����

    for (int i = 1; i <= order; i++) { //��������� ����� �� ������

        float* stack_grid = createGrid(seed, pow(startScale, i));//������� ������
        float step = (pow(startScale, i)+1) / 64; //������� ����������� �� ������ ������������� ���������� �� arr2d

        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 64; y++) {
                (*heightMap)[x][y] += (getNoiseValue(x * step, y * step, stack_grid, pow(startScale, i))/pow(startScale, i - 1)); //���� �������� �� 1
            }
         }

        delete(stack_grid);//��������� ������
    }
    
    for (int x = 0; x < 64; x++) { // �����
        for (int y = 0; y < 64; y++) {
            (*heightMap)[x][y] /= low;
        }
    }
    
    return heightMap;

}