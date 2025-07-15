#include <fstream>
#include <iostream>

void save(int seed) {
	std::fstream saveFile;//����� ������-������
	saveFile.open("Saves/save.txt", std::ios::out); //std::ios::app ��� ��������
	saveFile << seed;
	std::cout << "save " << seed << "\n";
	saveFile.close();
}

int load() { //����� ��������
	std::fstream saveFile;//����� ������-������
	saveFile.open("Saves/save.txt", std::ios::in); //std::ios::app ��� ��������
	int seed;
	saveFile >> seed;
	saveFile.close();
	std::cout << "load " << seed << "\n";
	return seed;
}