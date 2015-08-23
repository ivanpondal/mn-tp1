#ifndef ALTO_HORNO_H_
#define ALTO_HORNO_H_

#define PI 3.14159 //26535897932384626433832795

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <iomanip>

#include <sistema_ecuaciones.h>

enum TipoIsoterma : int {BINARIA = 0, AVG = 1, POL_FIT = 2};

using namespace std;

class AltoHorno{
	public:
		AltoHorno(const char* entrada);
		void generarSoluciones(const char* salida, TipoResolucion tipo);
		vector<double> calcularIsoterma(TipoIsoterma tipo);
		vector<pair<vector<double>, vector<double> > > darInstancias();
		vector<vector<double> > darSoluciones();

	private:
		void cargar(istream& entrada);
		void guardar(ostream& salida, vector<double> &x);
		void generarSistema();
		double jesimoRadio(int j);
		double kesimoAngulo(int k);
		vector<double> calcularIsotermaAVG(int instancia);
		vector<double> calcularIsotermaBinaria(int instancia);
		double radioInterior;
		double radioExterior;
		int cantParticiones;
		int cantAngulos;
		double isoterma;
		int cantInstancias;
		vector<pair<vector<double>, vector<double> > > instancias;
		vector<vector<double> > soluciones;
		SistemaEcuaciones sistemaTemperaturas;
};

#endif // ALTO_HORNO_H_INCLUDED
