#ifndef CMARKOVCHAIN_HPP
#define CMARKOVCHAIN_HPP

// Includes
#include<exception>
#include<iterator>
#include<iostream>
#include<random>
#include<string>
#include<map>

#define SAFE_DELETE(X) {if(X!=nullptr){delete X; X=nullptr;}}

// -- cMarkovChain --
class cMarkovChain {
public:

	// nGram-Struktur
	struct ngram_data {
		int ap_times;   /// Anzahl wie oft das nGram vorkommt
		bool begin;     /// Legt fest ob ein Name mit diesem nGram beginnen kann
		int chars_size; /// Größe der chars-Map
		std::map<char, int> chars; /// Chars die auf das aktuelle nGram folgen können
	};

	// -- Kon/Destruktor --
	cMarkovChain();  /// Konstruktor
	~cMarkovChain(); /// Destruktor

	// -- Methoden --
	void genNGrams(std::vector<std::string> &texts); /// Generiert alle möglichen ngrams eines String Vectors
	std::string genName(); /// Methode generiert einen Namen

private:

	// -- Member Vars --
	int *_order; /// Legt die Größe der NGrams fest
	
	std::map<std::string, ngram_data> *_ngrams; /// Map das alle nGrams und deren chars speichert
	std::map<std::string, ngram_data>::iterator _ngIT; /// nGram Iterator

	std::map<char, int>::iterator _cIT; /// Char Iterator
	
	int *_bgNGramsNum; /// Anzahl aller nGrams deren begin-Flag == true

	// -- Member Methods --
	void _addChar(std::string &ngram, char c); /// Fügt einem nGram einen "Nachfolgecharakter" hinzu
	int  _genRandNum(unsigned int min, unsigned int max); /// Zufällige Nummer generieren
};

#endif