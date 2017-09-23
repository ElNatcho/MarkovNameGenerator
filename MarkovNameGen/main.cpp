#include<iostream>
#include"common.hpp"
#include"cMarkovChain.hpp"

int main(int argc, char **argv) {
	/// Prüfen ob alle Kommandozeilenparameter vorhanden sind
	if (argc != 3) {
		std::cout << "MarkovNameGen.exe <Name_File> <Name_Num>" << std::endl;
		return 0;
	}

	cMarkovChain mc = cMarkovChain(); /// Markow-Kette erstellen
	mc.genNGrams(loadNamesFromFile(argv[1])); /// Beispiel-Namen aus Datei lesen
	
	/// Bestimmte Anzahl an Namen generieren
	for (int i = 0; i < std::stoi(argv[2]); i++)
		std::cout << "<" << i << "> " + mc.genName() << std::endl; /// Namen generieren


	return 0;
}