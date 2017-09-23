#include"cMarkovChain.hpp"

// -- Konstruktor --
cMarkovChain::cMarkovChain() {
	/// Alloc Mem
	_order = new int;
	_ngrams = new std::map<std::string, ngram_data>();
	_bgNGramsNum = new int;

	/// Werte setzen
	*_order = 3;
	*_bgNGramsNum = 0;
}

//  -- genNGrams --
/// Generiert alle möglichen ngrams eines String Vectors
/// @param texts: String-Vector von welchen die nGrams generiert werden sollen
void cMarkovChain::genNGrams(std::vector<std::string> &texts) {
	for (int i = 0; i < texts.size(); i++) { /// Durch alle Namen iterieren
		if (texts.at(i).size() >= *_order) { /// Testen ob der Name lang genug ist um ein nGram zu sein
			for (int j = 0; j < texts.at(i).size() - (*_order - 1); j++) { /// Durch alle nGrams eines Strings iterieren
				_ngIT = _ngrams->find(texts.at(i).substr(j, *_order)); /// Nach aktuellen nGram suchen
				if (_ngIT != _ngrams->end()) { /// Prüfen ob das nGram bereits vorhanden ist
					_ngIT->second.ap_times++; /// "Vorkommensanzahl" inkrementieren
				} else { /// Falls es noch nicht vorhanden ist ...
					_ngrams->insert( /// ... nGram einfügen ...
						std::make_pair( /// ... Pair erstellen ...
							texts.at(i).substr(j, *_order), /// ... nGram als Substring ermitteln ...
							ngram_data{ 1, j == 0 ? true : false, 0})); /// ... und das begin-flag ermitteln
					if (j == 0) *_bgNGramsNum += 1; /// Erneut prüfen ob das begin-Flag gesetzt wurde. Falls ja Variable inkrementieren
				}				
				/// Char der nach dem nGram kommt hinzufügen
				/// Prüfen ob es ein Buchstabe oder ein Leerzeichen ist
				if (j + *_order < texts.at(i).size()) { /// Nächstes Zeichen ist ein Buchstabe
					_addChar(texts.at(i).substr(j, *_order), texts.at(i).at(j + *_order));
				} else { /// Nächstes zeichen ist ein Leerzeichen
					_addChar(texts.at(i).substr(j, *_order), ' '); /// Leerzeichen hinzufügen 
				}
			}
		}
	}
}

//  -- genName --
/// Methode generiert einen Namen
std::string cMarkovChain::genName() {
	std::string name = ""; /// Variable in der der Name zwischengespeichert wird
	int rand_num = _genRandNum(0, *_bgNGramsNum); /// Zufällige Nummer erstellen
	int cur_count = 0; /// Zähl-Variable

	/// Anfang des Namen ermitteln
	for (_ngIT = _ngrams->begin(); _ngIT != _ngrams->end(); _ngIT++) { /// Durch alle Einträge iterieren
		if (_ngIT->second.begin) { /// Prüfen ob der aktuelle Eintrag ein Beginner ist
			cur_count++; /// cur_count inkrementieren
			if (cur_count >= rand_num) { /// Wenn der cur_count nun größer/gleich der zufälligen Zahl ist haben wir den Anfang gefunden
				name += _ngIT->first; /// Anfang zur temporären Variable hinzufügen
				break; /// Loop beenden
			}
		}
	}
	/// Rest des Namen generieren
	/// Solange ausführen bis der Name zu ende ist oder bis er länger als 15 Zeichen ist
	while (name.at(name.size() - 1) != ' ' && name.size() < 15) {
		_ngIT = _ngrams->find(name.substr(name.size() - *_order)); /// nGram das am Ende des Namen ist ausfindig machen
		if (_ngIT != _ngrams->end()) { /// Prüfen ob das nGram vorhanden ist
			rand_num = _genRandNum(0, _ngIT->second.chars_size); /// Zufällig Zahl für die Char-Auswahl erstellen
			cur_count = 0; /// Count zurücksetzen
			for (_cIT = _ngIT->second.chars.begin(); _cIT != _ngIT->second.chars.end(); _cIT++) { /// Durch die aktuelle char liste iterieren
				cur_count += _cIT->second; /// cur_count mit der "Auftretungswahrscheinlichkeit" des aktuellen chars summieren
				if (cur_count >= rand_num) { /// Wenn der cur_count nun größer/gleich der zufällig Zahl ist haben wir das nächste zeichen gefunden
					name += _cIT->first; /// Neues Char hinzufügen
					break; /// For-Loop beenden
				}
			}
		} else { /// Falls nicht Fehler werfen
			throw std::exception(("ERR: nGram \"" + name.substr(name.size() - *_order) + "\" ist nicht vorhanden.").c_str());
		}
	}

	return name; /// Name zurückgeben
}

//  -- _addChar --
/// Fügt einem nGram einen "Nachfolgecharakter" hinzu
/// @param _ngram: nGram dem der Charakter hinzugefügt werden soll
/// @param c     : Charakter der hinzugefügt werden soll
void cMarkovChain::_addChar(std::string &ngram, char c) {
	_ngIT = _ngrams->find(ngram); /// nGram ausfindig machen
	if (_ngIT != _ngrams->end()) { /// Prüfen ob das nGram vorhanden ist
		_cIT = _ngIT->second.chars.find(c); /// Char ausfindig machen
		if (_cIT != _ngIT->second.chars.end()) { /// Prüfen ob der Char bereits vorhanden ist
			_cIT->second++; /// "Vorkommensanzahl" inkrementieren
		} else { /// Falls nicht
			_ngIT->second.chars.insert(std::make_pair(c, 1)); /// Neuen Char einfügen
		}
		_ngIT->second.chars_size++; /// Größe der chars-Map inkrementieren
	} else { /// Falls nicht Fehler werfen
		throw std::exception(("ERR: nGram \"" + ngram + "\" existiert nicht!").c_str());
	}
}

//  -- _genRandNum --
/// Methode erstellt eine zufällig Nummer
/// @param min, max: Reichweite in der die Nummer generiert werden soll
int cMarkovChain::_genRandNum(unsigned int min, unsigned int max) {
	std::random_device rd; /// "Seed-Engine"
	std::mt19937 rng(rd()); /// "Random-Number Engine" (Mersenne-Twister)
	std::uniform_real_distribution<float> dist((float)min, (float)max); /// Range erstellen
	return static_cast<int>(dist(rng)); /// Zufällig Nummer erstellen und zurückgeben
}

// -- Destruktor --
cMarkovChain::~cMarkovChain() {
	/// Free Mem
	SAFE_DELETE(_order);
	_ngrams->clear();
	SAFE_DELETE(_ngrams);
	SAFE_DELETE(_bgNGramsNum);
}