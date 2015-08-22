#ifndef SISTEMA_ECUACIONES_H_
#define SISTEMA_ECUACIONES_H_

enum TipoResolucion : bool {GAUSS = 0, LU = 1};

#include <iostream>
#include <vector>

using namespace std;

class SistemaEcuaciones{
	public:
		SistemaEcuaciones();
		SistemaEcuaciones(vector<vector<double> > A, vector<vector<double> > instB, int dimMatriz, int cantAngulos);
		vector<double> resolverSistema(int instancia, TipoResolucion tipo);
	private:
		vector<vector<double> > A;
		vector<vector<double> > instB;
		vector<vector<double> > factorizacionLU;
		int dimMatriz;
		int cantAngulos;
		vector<double> resolverTriangular(vector<vector<double> > &U, vector<double> &b, int n);
		void eliminacionGaussiana(vector<vector<double> > &A, vector<double> &b, int n);
		void eliminacionGaussianaBanda(vector<vector<double> > &A, vector<double> &b, int n, int cantAngulos);
		void factorizarLU(vector<vector<double> > &A, vector<double> &b, int n);
		void imprimirSistema(vector<vector<double> > &mA, vector<double> &b );
};

#endif // SISTEMA_ECUACIONES_H_INCLUDED
