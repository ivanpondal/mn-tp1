#ifndef ALTO_HORNO_H_
#define ALTO_HORNO_H_

#include <fstream>
#include <utility>
#include <map>
#include <vector>

using namespace std;

class AltoHorno{
	public:
		AltoHorno(const char* entrada, bool lu);
		int darRadioInterior();
		int darRadioExterior();
		int darCantParticiones();
		int darCantAngulos();
		int darIsoterma();
		int darCantInstancias();
		map<int, pair<vector<int>, vector<int> > > darInstancias();

	private:
		void cargar(istream& entada, bool lu);
		int radioInterior;
		int radioExterior;
		int cantParticiones;
		int cantAngulos;
		int isoterma;
		int cantInstancias;
		map<int, pair<vector<int>, vector<int> > > instancias;
		int usaLU;
};

#endif // ALTO_HORNO_H_INCLUDED
