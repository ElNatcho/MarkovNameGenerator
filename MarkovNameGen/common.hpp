#ifndef COMMON_HPP
#define COMMON_HPP

// Includes
#include<exception>
#include<fstream>
#include<vector>
#include<string>

// SAFE_DELETE Makro
#define SAFE_DELETE(X) {if(X!=nullptr){delete X; X=nullptr;}}

// -- loadNamesFromFile --
/// Methoden liest Namen aus einer Datei als String aus und speichert sie in einem Vector
/// @param path: Pfad zur/Name der Datei
std::vector<std::string> loadNamesFromFile(std::string path) {
	std::vector<std::string> tmpSVec; /// Tempor�res String-Vector Objekt um die Zeilen aus der Datei zu speichern
	std::ifstream ifstr; /// Ifstream um die Strings aus der Datei auszulesen
	std::string tmpStr;  /// Tempor�rer String um die Strings aus der Datei auszulessen

	/// Datei �ffnen
	ifstr.open(path, std::ios::in);
	if (!ifstr.is_open()) { /// Testen ob die Datei ge�ffnet wurde
		throw std::exception(("ERR: Datei \"" + path + "\" kann nicht ge�ffnet werden!").c_str()); /// Exception werfen
	}

	/// Alle Daten aus der Datei auslesen
	while (!ifstr.eof()) { /// Solange ausf�hren bis das Ende der Datei erreicht ist
		std::getline(ifstr, tmpStr); /// Aktuelle Zeile auslesen
		/// Sollte der String aus mehreren Namen bestehen -> String trennen
		for (int i = 0; i < tmpStr.length(); i++) { /// Durch jedes Zeichen des Strings iterieren
			if (tmpStr.at(i) == ' ') { /// Testen ob das aktuelle Zeichen ein Leerzeichen ist
				tmpSVec.push_back(tmpStr.substr(0, i)); /// Aktuellen Namen einf�gen
				tmpStr = tmpStr.substr(i + 1); /// Aktuellen Namen l�schen
				i = 0; /// Zur�cksetzen um von anfang an zu beginnen
			}
		}
		tmpSVec.push_back(tmpStr); /// Den letzten Namen der Zeile einf�gen
	}

	/// Den ausgelesenen String-Vector zur�ckgeben
	return tmpSVec;
}

#endif