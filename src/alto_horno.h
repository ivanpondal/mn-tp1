#ifndef ALTO_HORNO_H_
#define ALTO_HORNO_H_

#define PI 3.14159 //26535897932384626433832795

#include <iostream>
#include <fstream>
#include <utility>
#include <map>
#include <vector>

#include <sistema_ecuaciones.h>

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
		void calcularTemperaturas(int instancia);

	private:
		void cargar(istream& entada, bool lu);
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
		int usaLU;
		SistemaEcuaciones sistemaTemperaturas;
};

#endif // ALTO_HORNO_H_INCLUDED
