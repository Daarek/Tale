#include <fstream>
#include <iostream>

void save(int seed) {
	std::fstream saveFile;//поток чтения-записи
	saveFile.open("Saves/save.txt", std::ios::out); //std::ios::app для дозаписи
	saveFile << seed;
	std::cout << "save " << seed << "\n";
	saveFile.close();
}

int load() { //потом поменять
	std::fstream saveFile;//поток чтения-записи
	saveFile.open("Saves/save.txt", std::ios::in); //std::ios::app для дозаписи
	int seed;
	saveFile >> seed;
	saveFile.close();
	std::cout << "load " << seed << "\n";
	return seed;
}