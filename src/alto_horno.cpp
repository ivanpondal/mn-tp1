#include "alto_horno.h"

AltoHorno::AltoHorno(const char* entrada, bool lu){
	ifstream archivoEntrada;
	archivoEntrada.open(entrada);

	cargar(archivoEntrada, lu);
	generarSistema();

	archivoEntrada.close();
}

void AltoHorno::cargar(istream& entrada, bool lu){
	entrada >> this->radioInterior;
	entrada >> this->radioExterior;
	entrada >> this->cantParticiones;
	entrada >> this->cantAngulos;
	entrada >> this->isoterma;
	entrada >> this->cantInstancias;
	this->instancias = vector<pair<vector<double>, vector<double> > >(cantInstancias);
	for(int i = 0; i < this->cantInstancias; i++){
		double tmp;
		vector<double> tempInterior(this->cantAngulos), tempExterior(this->cantAngulos);
		for(int j = 0; j < this->cantAngulos; j++){
			entrada >> tmp;
			tempInterior[j] = tmp;
		}
		for(int j = 0; j < this->cantAngulos; j++){
			entrada >> tmp;
			tempExterior[j] = tmp;
		}
		this->instancias[i] = pair<vector<double>, vector<double> >(tempInterior, tempExterior);
	}
	usaLU = lu;
}

void AltoHorno::guardar(ostream& salida, vector<double> &x){
	int dimMatriz = this->cantParticiones*this->cantAngulos;
	for(int i = 0; i < dimMatriz; i++){
		salida << setprecision(25) << x[i] << endl;
	}
}

void AltoHorno::generarSistema(){
	int n = this->cantAngulos;
	int m = this->cantParticiones;
	int dimMatriz = n*m;

	// inicializo matriz A y vectores b en 0

	vector<vector<double> > A(dimMatriz, vector<double>(dimMatriz, 0));
	vector<vector<double> > instB(dimMatriz, vector<double>(dimMatriz, 0));

	// temperaturas internas
	for(int f = 0; f < n; f++){
		A[f][f] = 1;
		for(int i = 0; i < this->cantInstancias; i++){
			instB[i][f] = this->instancias[i].first[f];
		}
	}

	// temperaturas interior
	int j, k;
	double beta, gamma, alpha, rj, difR, difA;
	for(int f = n; f < dimMatriz - n; f++){
		j = f / n;
		k = f - n*j;
		rj = jesimoRadio(j);
		difR = rj - jesimoRadio(j - 1);
		difA = kesimoAngulo(k) - kesimoAngulo(k - 1);

		beta = 1/(difR*difR);
		gamma = 1/(difR*rj);
		alpha = 1/(difA*difA*rj*rj);

		A[f][f] = gamma - 2*beta - 2*alpha;
		A[f][f - n] = beta - gamma;
		A[f][f + n] = beta;
		A[f][f + (((k - 1) == -1) ? n-1 : -1)] = alpha;
		A[f][f + (((k + 1) == n) ? 1-n : 1)] = alpha;
	}

	// temperaturas externas
	for(int f = dimMatriz - n; f < dimMatriz; f++){
		k = f - dimMatriz + n;
		A[f][f] = 1;
		for(int i = 0; i < this->cantInstancias; i++){
			instB[i][f] = this->instancias[i].second[k];
		}
	}

	this->sistemaTemperaturas = SistemaEcuaciones(A, instB, dimMatriz, cantAngulos);
}

double AltoHorno::jesimoRadio(int j){
	return this->radioInterior + j*((this->radioExterior - this->radioInterior)/(this->cantParticiones - 1));
}

double AltoHorno::kesimoAngulo(int k){
	return 2*PI*((k + this->cantAngulos) % this->cantAngulos)/this->cantAngulos;
}

vector<pair<vector<double>, vector<double> > > AltoHorno::darInstancias(){
	return this->instancias;
}

void AltoHorno::generarSoluciones(const char* salida){
	ofstream archivoSalida;
	archivoSalida.open(salida);

	vector<double> x;
	for(int i = 0; i < this->cantInstancias; i++){
		x = sistemaTemperaturas.resolverSistema(i, usaLU);
		guardar(archivoSalida, x);
	}

	archivoSalida.close();
}
