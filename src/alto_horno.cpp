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
		salida<<x[i]<<endl;
	}
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

		matrizA[f][f] = gamma - 2*beta + 2*alpha;
		matrizA[f][f - n] = beta - gamma;
		matrizA[f][f + n] = beta;
		matrizA[f][f + (((k - 1) == -1) ? n-1 : -1)] = alpha;
		matrizA[f][f + (((k + 1) == n) ? 1-n : 1)] = alpha;
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
