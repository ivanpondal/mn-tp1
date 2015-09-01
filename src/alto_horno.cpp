#include "alto_horno.h"

AltoHorno::AltoHorno(const char* entrada){
	ifstream archivoEntrada;
	archivoEntrada.open(entrada);

	if (archivoEntrada.good()) {
		cargar(archivoEntrada);
		generarSistema();
	} else {
		cout << endl;
		cout << "\tNo se pudo cargar el horno definido por el archivo: " << entrada << endl;
	}

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

void AltoHorno::guardar(ostream& salida, vector<double> x){
	for(int i = 0; i < (int)x.size(); i++){
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

vector<vector<double> > AltoHorno::darSoluciones(){
	return this->soluciones;
}

vector<pair<vector<double>, vector<double> > > AltoHorno::darInstancias(){
	return this->instancias;
}

SistemaEcuaciones AltoHorno::darSistema(){
	return this->sistemaTemperaturas;
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

void AltoHorno::resolverSistema(TipoResolucion tipo){

	for(int i = 0; i < this->cantInstancias; i++){
		soluciones[i] = sistemaTemperaturas.resolverSistema(i, tipo);
	}

}

void AltoHorno::escribirIsoterma(const char* salida, TipoIsoterma tipo) {
	ofstream archivoSalida;
	archivoSalida.open(salida);
	guardar(archivoSalida, calcularIsoterma(tipo));
	archivoSalida.close();
}

vector<double> AltoHorno::calcularIsoterma(TipoIsoterma tipo) {
	if (tipo == AVG) {
		return calcularIsotermaAVG(0);
	} else if (tipo == BINARIA){
		return calcularIsotermaBinaria(0);
	} else if (tipo == LINEAR_FIT){
		return calcularIsotermaLinearFit(0);
	} else {
		return vector<double>();
	}
}

vector<double> AltoHorno::calcularIsotermaBinaria(int instancia){
	vector<double> solucion = vector<double>(cantAngulos);
	for(int angulo = 0; angulo < cantAngulos; ++angulo) {
		int radio = 0;
		while (radio < cantParticiones) {
			if (soluciones[instancia][radio*cantAngulos+angulo] < isoterma) break;
			++radio;
		}
		if (radio==0) {
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
				b[0][i] = soluciones[instancia][(radio-1)*cantAngulos+angulo];
				A[i+2*n][i+2*n] = 1;
				b[0][i+2*n] = soluciones[instancia][radio*cantAngulos+angulo];
			}
			double temperaturaActual = soluciones[instancia][(radio-1)*cantAngulos+angulo];
			while (fabs(temperaturaActual-isoterma) > 0.0001){
				double beta, gamma, alpha, radioMedio = (radioActual+radioSiguiente)/2.0, difR = radioMedio - radioActual, difA;
				for(int f = n; f < 2*n; ++f){
					int k = f-n;
					difA = kesimoAngulo(k) - kesimoAngulo(k - 1);

					beta = 1/(difR*difR);
					gamma = 1/(difR*radioMedio);
					alpha = 1/(difA*difA*radioMedio*radioMedio);

					A[f][f] = gamma - 2*beta - 2*alpha;
					A[f][f - n] = beta - gamma;
					A[f][f + n] = beta;
					A[f][f + (((k - 1) == -1) ? n-1 : -1)] = alpha;
					A[f][f + (((k + 1) == n) ? 1-n : 1)] = alpha;
				}
				SistemaEcuaciones sistema = SistemaEcuaciones(A, b, dimension, n);
				vector<double> solucionAuxiliar = sistema.resolverSistema(0, GAUSS); // no tiene sentido usar LU (solo quiero resolverlo para un b)
				double temperaturaMedia = solucionAuxiliar[angulo+n];
				if (temperaturaMedia < isoterma) {
					radioSiguiente = radioMedio;
					for (int i = 0; i < n; ++i){
						b[0][i+2*n] = solucionAuxiliar[i+n];
					}
				} else {
					radioActual = radioMedio;
					temperaturaActual = temperaturaMedia;
					for (int i = 0; i < n; ++i){
						b[0][i] = solucionAuxiliar[i+n];
					}
				}
			}
			solucion[angulo] = radioActual;
		}
	}
	return solucion;
}

vector<double> AltoHorno::calcularIsotermaAVG(int instancia) {
	vector<double> solucion = vector<double>(cantAngulos);
	for(int angulo = 0; angulo < cantAngulos; ++angulo) {
		double lower_bound_iso = 0;
		double upper_bound_iso = 0;
		for(int radio = 0; radio < cantParticiones-1; ++radio) {
			lower_bound_iso = soluciones[instancia][radio*cantParticiones+angulo];
			upper_bound_iso = soluciones[instancia][(radio+1)*cantParticiones+angulo];
			if (lower_bound_iso >= isoterma && upper_bound_iso <= isoterma) {
				//cout << "angulo: " << angulo << " -> entre: " << jesimoRadio(radio) << "-" << jesimoRadio(radio+1) << endl;
				solucion[angulo] = (jesimoRadio(radio) + jesimoRadio(radio+1))/2.0;
			}
		}
	}
	return solucion;
}

vector<double> AltoHorno::calcularIsotermaLinearFit(int instancia) {
	vector<double> solucion = vector<double>(cantAngulos);
	for(int angulo = 0; angulo < cantAngulos; ++angulo) {
		// calcular linear fit
		double avgX = 0;
		double avgY = 0;
		for(int radio = 0; radio < cantParticiones; ++radio) {
			avgX += jesimoRadio(radio);
			avgY += soluciones[instancia][radio*cantAngulos+angulo];
		}
		avgX /= cantParticiones;
		avgY /= cantParticiones;

		double slope_numerator = 0.0;
		double slope_denominator = 0.0;

		for(int radio = 0; radio < cantParticiones; ++radio){
			double x = jesimoRadio(radio);
			double y = soluciones[instancia][radio*cantAngulos+angulo];
			slope_numerator += (x - avgX) * (y - avgY);
			slope_denominator += (x - avgX) * (x - avgX);
		}

		if(slope_denominator == 0){
			cout << "slope_denominator in Linear Regression is zero" << endl;
			slope_denominator = 1;
		}

		double slope = slope_numerator / slope_denominator;
		double intercept = avgY - slope * avgX;
		// cout << "slope: " << slope << endl;
		// cout << "intercept: " << intercept << endl;

		solucion[angulo] = fabs((isoterma - intercept) / slope);
	}
	return solucion;
}

double AltoHorno::calcularNumeroCondicion() {
	return sistemaTemperaturas.calcularNumeroCondicion();
}

bool AltoHorno::evaluarEstructura(const vector<double> &isoterma, double epsilon, TipoEvaluacion tipo) {
	// calculo los deltas para cada angulo
	vector<double> deltas(cantAngulos);
	bool delta_mayor_epsilon = false;
	double aux = radioExterior - radioInterior;
	for(int i = 0; i < cantAngulos; i++) {
		double delta = (radioExterior - isoterma[i])/aux;
		deltas[i] = delta;
		if (epsilon >= delta) {
			delta_mayor_epsilon = true;
		}
	}

	if(tipo == SIMPLE) {
		return delta_mayor_epsilon;
	} else {
		// tomo el promedio de los deltas
		double sum = 0;
		for(int i = 0; i < cantAngulos; i++) {
			sum += deltas[i];
		}
		sum = sum / cantAngulos;
		return epsilon >= sum;
	}
}
