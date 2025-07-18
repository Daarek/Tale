#include <fstream>
#include <iostream>

void save(int seed) {
	std::fstream saveFile;//����� ������-������
	saveFile.open("Saves/save.txt", std::ios::out); //std::ios::app ��� ��������
	saveFile << seed;
	saveFile.close();
}

int load() { //����� ��������
	std::fstream saveFile;//����� ������-������
	saveFile.open("Saves/save.txt", std::ios::in); //std::ios::app ��� ��������
	int seed;
	saveFile >> seed;
	saveFile.close();
	return seed;
}