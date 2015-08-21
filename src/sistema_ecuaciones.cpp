#include <sistema_ecuaciones.h>

#define BANDA true

SistemaEcuaciones::SistemaEcuaciones(){
	this->dimMatriz = 0;
}

SistemaEcuaciones::SistemaEcuaciones(vector<vector<double> > matrizA, map<int, vector<double> > instB, int dimMatriz, int cantAngulos){
	this->matrizA = matrizA;
	this->instB = instB;
	this->dimMatriz = dimMatriz;
	this->cantAngulos = cantAngulos;
}

vector<double> SistemaEcuaciones::resolverSistema(int instancia, bool lu){
	// copio por si luego quisiera usar otro método
	vector<vector<double> > mA = this->matrizA;
	vector<double> b = this->instB[instancia];

	if(lu){
		// resolver utilizando factorización lu
		return vector<double>(this->dimMatriz, 0);
	}
	else{
		if (BANDA) {
			eliminacionGaussianaBanda(mA, b, this->dimMatriz, this->cantAngulos);
			return resolverTriangular(mA, b, this->dimMatriz);	
		} else {
			eliminacionGaussiana(mA, b, this->dimMatriz);
			return resolverTriangular(mA, b, this->dimMatriz);	
		}
	}
}

void SistemaEcuaciones::eliminacionGaussiana(vector<vector<double> > &matrizA, vector<double> &b, int n){
	// encuentro matriz triangular inferior:
	for(int i = 0; i < n; i++){
		double pivote = matrizA[i][i];
		for(int j = i+1; j < n; j++){
			// sabemos de antemano que el pivote no es cero.
			double coef = matrizA[j][i] / pivote;
			// modifico la fila j usando la fila i
			matrizA[j][i] = 0;
			for(int k = i+1; k < n; k++){
				matrizA[j][k] -= coef * matrizA[i][k];
			}
			b[j] -= coef * b[i];
		}
	}
}

void SistemaEcuaciones::eliminacionGaussianaBanda(vector<vector<double> > &matrizA, vector<double> &b, int n, int cantAngulos){
	// encuentro matriz triangular inferior:

	// sabemos  que las ecuaciones de los puntos t(0,0) a t(0,n-1) y t(m,0) a t(m,n-1) ya tienen ceros en toda la fila (excepto en la diagonal, que es uno)
	// empezamos a laburar en las ecuaciones t(1,0), y terminamos en la ecuacion t(m-1, n-1)
	for(int i = 0; i < n; i++){
		double pivote = matrizA[i][i];
		
		int inicioBanda = max(i+1,cantAngulos);
		int finBanda = min(n,inicioBanda + cantAngulos);

		for(int j = inicioBanda; j < finBanda; j++){
			if (matrizA[j][i] != 0) {
				// modifico la fila j usando la fila i
				double coef = matrizA[j][i] / pivote;

				matrizA[j][i] = 0;
				for(int k = i+1; k < n; k++){
					matrizA[j][k] -= coef * matrizA[i][k];
				}
				b[j] -= coef * b[i];
			}
		}
	}
}

vector<double> SistemaEcuaciones::resolverTriangular(vector<vector<double> > &matrizU, vector<double> &b, int n){
	vector<double> x(n, 0);
	for(int i = n-1; i >= 0; --i) {
		for(int j = i+1; j < n; j++) {
			b[i] -= matrizU[i][j] * x[j];
		}
		x[i] = b[i] / matrizU[i][i];
	}
	return x;
}

void SistemaEcuaciones::factorizacionLU(vector<vector<double> > &matrizA, vector<double> &b, int n){
	vector<vector<double> > L(n, vector<double>(n, 0));
	//obtener L, U va a ser lo que quede en matrizA
	for (int i = 0; i < n-1; ++i) {//fila
		L[i][i] = 1;
		for (int j = i+1; j < n; ++j) {//fila
			double c = A[j][i]/A[i][i];
			for (int k = i; k < n; ++k) {//columnas
				
			}
		}
	}
}

void SistemaEcuaciones::imprimirSistema(vector<vector<double> > &mA, vector<double> &b){
	cout<<endl;
	for(int j = 0; j < dimMatriz; j++){
		cout<<"[";
		for(int k = 0; k < dimMatriz; k++){
			cout<<" "<<mA[j][k];
		}
		cout<<" | "<<b[j]<<" ]"<<endl;
	}
}
