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
		double darRadioInterior();
		double darRadioExterior();
		int darCantParticiones();
		int darCantAngulos();
		double darIsoterma();
		int darCantInstancias();
		map<int, pair<vector<double>, vector<double> > > darInstancias();

	private:
		void cargar(istream& entada, bool lu);
		double radioInterior;
		double radioExterior;
		int cantParticiones;
		int cantAngulos;
		double isoterma;
		int cantInstancias;
		map<int, pair<vector<double>, vector<double> > > instancias;
		int usaLU;
};

#endif // ALTO_HORNO_H_INCLUDED
