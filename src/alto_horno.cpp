#include "alto_horno.h"
#include <math.h>

AltoHorno::AltoHorno(const char* entrada){
	ifstream archivoEntrada;
	archivoEntrada.open(entrada);

	cargar(archivoEntrada);
	generarSistema();

	archivoEntrada.close();
}

void AltoHorno::cargar(istream& entrada){
	entrada >> this->radioInterior;
	entrada >> this->radioExterior;
	entrada >> this->cantParticiones;
	entrada >> this->cantAngulos;
	entrada >> this->isoterma;
	entrada >> this->cantInstancias;
	this->instancias = vector<pair<vector<double>, vector<double> > >(cantInstancias);
	this->soluciones = vector<vector<double> >(cantInstancias);
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

void AltoHorno::generarSoluciones(const char* salida, TipoResolucion tipo){
	ofstream archivoSalida;
	archivoSalida.open(salida);

	vector<double> x;
	for(int i = 0; i < this->cantInstancias; i++){
		soluciones[i] = sistemaTemperaturas.resolverSistema(i, tipo);
		guardar(archivoSalida, soluciones[i]);
	}

	archivoSalida.close();
}

vector<double> AltoHorno::calcularIsoterma(TipoIsoterma tipo) {
	return calcularIsotermaBinaria(0);
}

vector<double> AltoHorno::calcularIsotermaBinaria(int instancia){
	vector<double> solucion = vector<double>(cantAngulos);
	for(int angulo = 0; angulo < cantAngulos; ++angulo) {
		int radio = 0;
		while (radio < cantParticiones) {
			if (soluciones[instancia][radio*cantParticiones+angulo] > isoterma) break;
			++radio;
		}
		if (radio==0) {
			cout << "llego" << endl;
			solucion[instancia] = jesimoRadio(radio);
		} else if (radio == cantParticiones) {
			solucion[instancia] = jesimoRadio(cantParticiones-1);
		} else {
			int n = this->cantAngulos;
			double radioActual = jesimoRadio(radio-1);
			double radioSiguiente = jesimoRadio(radio);
			int dimension = 3*n;
			vector<vector<double> > A = vector<vector<double> >(dimension, vector<double>(dimension, 0));
			vector<vector<double> > b = vector<vector<double> >(1, vector<double>(dimension, 0));
			for (int i = 0; i < n; i++){
				A[i][i] = 1;
				b[0][i] = instancias[instancia].first[i];
				A[dimension-1-i][dimension-1-i] = 1;
				b[0][dimension-1-i] = instancias[instancia].second[i];
			}
			while (fabs(radioActual-isoterma) > 0.1){
				double beta, gamma, alpha, rj, difR, difA;
				for(int f = n; f < 2*n; ++f){
					int k = f-n;
					rj = (radioActual+radioSiguiente)/2.0;
					difR = rj - radioActual;
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
				SistemaEcuaciones sistema = SistemaEcuaciones(A, b, dimension, n);
				vector<double> solucionAuxiliar = sistema.resolverSistema(0, GAUSS); // no tiene sentido usar LU (solo quiero resolverlo para un b)
				double nuevoRadio = solucionAuxiliar[angulo];
				if (nuevoRadio > isoterma) {
					radioSiguiente = nuevoRadio;
				} else {
					radioActual = nuevoRadio;
				}
			}
			solucion[angulo] = radioActual;
			cout << radioActual << endl;
		}
	}
	return solucion;
}
