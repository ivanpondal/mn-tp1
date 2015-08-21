#ifndef SISTEMA_ECUACIONES_H_
#define SISTEMA_ECUACIONES_H_

enum tipoResolucion : bool {GAUSS, LU};

#include <iostream>
#include <vector>

using namespace std;

class SistemaEcuaciones{
	public:
		SistemaEcuaciones();
		SistemaEcuaciones(vector<vector<double> > A, vector<vector<double> > instB, int dimMatriz, int cantAngulos);
		vector<double> resolverSistema(int instancia, bool lu);
	private:
		vector<vector<double> > A;
		vector<vector<double> > instB;
		int dimMatriz;
		int cantAngulos;
		void eliminacionGaussiana(vector<vector<double> > &A, vector<double> &b, int n);
		void eliminacionGaussianaBanda(vector<vector<double> > &A, vector<double> &b, int n, int cantAngulos);
		void factorizacionLU(vector<vector<double> > &A, vector<double> &b, int n);
		vector<double> resolverTriangular(vector<vector<double> > &U, vector<double> &b, int n);
		void imprimirSistema(vector<vector<double> > &mA, vector<double> &b );
};

#endif // SISTEMA_ECUACIONES_H_INCLUDED
