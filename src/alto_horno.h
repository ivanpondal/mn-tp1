#ifndef ALTO_HORNO_H_
#define ALTO_HORNO_H_

#define PI 3.14159 //26535897932384626433832795

#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <vector>
#include <iomanip>

#include <sistema_ecuaciones.h>

using namespace std;

class AltoHorno{
	public:
		AltoHorno(const char* entrada, bool lu);
		double darIsoterma();
		map<int, pair<vector<double>, vector<double> > > darInstancias();
		void generarSoluciones(const char* salida);

	private:
		void cargar(istream& entrada, bool lu);
		void guardar(ostream& salida, vector<double> &x);
		void generarSistema();
		double jesimoRadio(int j);
		double kesimoAngulo(int k);
		double radioInterior;
		double radioExterior;
		int cantParticiones;
		int cantAngulos;
		double isoterma;
		int cantInstancias;
		map<int, pair<vector<double>, vector<double> > > instancias;
		SistemaEcuaciones sistemaTemperaturas;
};

#endif // ALTO_HORNO_H_INCLUDED
