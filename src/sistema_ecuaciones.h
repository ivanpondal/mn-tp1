#ifndef SISTEMA_ECUACIONES_H_
#define SISTEMA_ECUACIONES_H_

#include <iostream>
#include <map>
#include <vector>

using namespace std;

class SistemaEcuaciones{
	public:
		SistemaEcuaciones();
		SistemaEcuaciones(vector<vector<double> > matrizA, map<int, vector<double> > instBi, int dimMatriz, int cantAngulos);
		vector<double> resolverSistema(int instancia, bool lu);
	private:
		vector<vector<double> > matrizA;
		map<int, vector<double> > instB;
		int dimMatriz;
		int cantAngulos;
		void eliminacionGaussiana(vector<vector<double> > &matrizA, vector<double> &b, int n);
		void eliminacionGaussianaBanda(vector<vector<double> > &matrizA, vector<double> &b, int n, int cantAngulos);
		vector<double> resolverTriangular(vector<vector<double> > &matrizU, vector<double> &b, int n);
		void imprimirSistema(vector<vector<double> > &mA, vector<double> &b );

};

#endif // SISTEMA_ECUACIONES_H_INCLUDED
