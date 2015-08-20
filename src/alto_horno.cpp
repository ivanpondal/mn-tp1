#include "alto_horno.h"

AltoHorno::AltoHorno(const char* entrada, bool lu){
	ifstream archivoEntrada;
	archivoEntrada.open(entrada);
	cargar(archivoEntrada, lu);
	generarSistema();
}

void AltoHorno::cargar(istream& entrada, bool lu){
	entrada >> this->radioInterior;
	entrada >> this->radioExterior;
	entrada >> this->cantParticiones;
	entrada >> this->cantAngulos;
	entrada >> this->isoterma;
	entrada >> this->cantInstancias;
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

void AltoHorno::generarSistema(){
	int n = this->cantAngulos;
	int m = this->cantParticiones;
	vector<vector<double> > matrizA;
	map<int, vector<double> > instB;
	int dimMatriz = n*m;

	// inicializo matriz A y vectores b en 0

	for(int i = 0; i < this->cantInstancias; i++){
		instB[i].assign(dimMatriz, 0);
	}

	matrizA.assign(dimMatriz, instB[0]);

	// temperaturas internas
	for(int f = 0; f < n; f++){
		matrizA[f][f] = 1;
		for(int i = 0; i < this->cantInstancias; i++){
			instB[i][f] = this->instancias[i].first[f];
		}
	}

	// temperaturas interior
	int j, k;
	double c0, c1, c2, rj, difR, difA;
	for(int f = n; f < dimMatriz - n; f++){
		j = f / n;
		k = f - n*j;
		rj = jesimoRadio(j);
		difR = rj - jesimoRadio(j - 1);
		difA = kesimoAngulo(k) - kesimoAngulo(k - 1);

		c0 = 1/(difR*difR);
		c1 = 1/(difR*rj);
		c2 = 1/(difA*difA*rj*rj);

		matrizA[f][f] = c1 - 2*c0 + 2*c2;
		matrizA[f][f - n] = c0 - c1;
		matrizA[f][f + n] = c0;
		matrizA[f][f + (((k - 1) == -1) ? n-1 : -1)] = c2;
		matrizA[f][f + (((k + 1) == n) ? 1-n : 1)] = c2;
	}

	// temperaturas externas
	for(int f = dimMatriz - n; f < dimMatriz; f++){
		k = f - dimMatriz + n;
		matrizA[f][f] = 1;
		for(int i = 0; i < this->cantInstancias; i++){
			instB[i][f] = this->instancias[i].second[k];
		}
	}

	this->sistemaTemperaturas = SistemaEcuaciones(matrizA, instB, dimMatriz);
}

double AltoHorno::jesimoRadio(int j){
	return this->radioInterior + j*((this->radioExterior - this->radioInterior)/(this->cantParticiones - 1));
}

double AltoHorno::kesimoAngulo(int k){
	return 2*PI*((k + this->cantAngulos) % this->cantAngulos)/this->cantAngulos;
}

double AltoHorno::darRadioInterior(){
	return this->radioInterior;
}

double AltoHorno::darRadioExterior(){
	return this->radioExterior;
}

int AltoHorno::darCantParticiones(){
	return this->cantParticiones;
}

int AltoHorno::darCantAngulos(){
	return this->cantAngulos;
}

double AltoHorno::darIsoterma(){
	return this->isoterma;
}

int AltoHorno::darCantInstancias(){
	return this->cantInstancias;
}

map<int, pair<vector<double>, vector<double> > > AltoHorno::darInstancias(){
	return this->instancias;
}

void AltoHorno::calcularTemperaturas(int instancia){
	sistemaTemperaturas.resolverSistema(instancia, usaLU);
	// TODO: guardar la solucion
}