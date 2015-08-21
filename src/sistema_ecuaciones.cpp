#include <sistema_ecuaciones.h>

#define BANDA true

SistemaEcuaciones::SistemaEcuaciones(){
	this->dimMatriz = 0;
}

SistemaEcuaciones::SistemaEcuaciones(vector<vector<double> > A, vector<vector<double> > instB, int dimMatriz, int cantAngulos){
	this->A = A;
	this->instB = instB;
	this->dimMatriz = dimMatriz;
	this->cantAngulos = cantAngulos;
}

vector<double> SistemaEcuaciones::resolverSistema(int instancia, bool lu){
	// copio por si luego quisiera usar otro método
	vector<vector<double> > mA = this->A;
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

void SistemaEcuaciones::eliminacionGaussiana(vector<vector<double> > &A, vector<double> &b, int n){
	// encuentro matriz triangular inferior:
	for(int i = 0; i < n; i++){
		double pivote = A[i][i];
		for(int j = i+1; j < n; j++){
			// sabemos de antemano que el pivote no es cero.
			double coef = A[j][i] / pivote;
			// modifico la fila j usando la fila i
			A[j][i] = 0;
			for(int k = i+1; k < n; k++){
				A[j][k] -= coef * A[i][k];
			}
			b[j] -= coef * b[i];
		}
	}
}

void SistemaEcuaciones::eliminacionGaussianaBanda(vector<vector<double> > &A, vector<double> &b, int n, int cantAngulos){
	// encuentro matriz triangular inferior:

	// sabemos  que las ecuaciones de los puntos t(0,0) a t(0,n-1) y t(m,0) a t(m,n-1) ya tienen ceros en toda la fila (excepto en la diagonal, que es uno)
	// empezamos a laburar en las ecuaciones t(1,0), y terminamos en la ecuacion t(m-1, n-1)
	for(int i = 0; i < n; i++){
		double pivote = A[i][i];
		
		int inicioBanda = max(i+1,cantAngulos);
		int finBanda = min(n,inicioBanda + cantAngulos);

		for(int j = inicioBanda; j < finBanda; j++){
			if (A[j][i] != 0) {
				// modifico la fila j usando la fila i
				double coef = A[j][i] / pivote;

				A[j][i] = 0;
				for(int k = i+1; k < n; k++){
					A[j][k] -= coef * A[i][k];
				}
				b[j] -= coef * b[i];
			}
		}
	}
}

vector<double> SistemaEcuaciones::resolverTriangular(vector<vector<double> > &U, vector<double> &b, int n){
	vector<double> x(n, 0);
	for(int i = n-1; i >= 0; --i) {
		for(int j = i+1; j < n; j++) {
			b[i] -= U[i][j] * x[j];
		}
		x[i] = b[i] / U[i][i];
	}
	return x;
}

void SistemaEcuaciones::factorizacionLU(vector<vector<double> > &A, vector<double> &b, int n){
	//obtener L y U, va a ser lo que quede en A
	for (int i = 0; i < dimMatriz-1; ++i) {//fila
		for (int j = i+1; j < dimMatriz; ++j) {//fila
			if (A[j][i] != 0) {
				double c = A[j][i]/A[i][i];
				A[j][i] = c;
				for (int k = i+1; k < n; ++k) {//columnas
					A[j][k] -= c*A[i][k];
				}
			}
		}
	}
	for (int i = 0; i < dimMatriz; ++i){
		for (int j = 0; j < dimMatriz; ++j){
			cout << A[i][j] << " ";
		}
		cout << endl;
	}
}

void SistemaEcuaciones::imprimirSistema(vector<vector<double> > &mA, vector<double> &b){
	for(int j = 0; j < dimMatriz; j++){
		cout<<"[";
		for(int k = 0; k < dimMatriz; k++){
			cout<<" "<<mA[j][k];
		}
		cout<<" | "<<b[j]<<" ]"<<endl;
	}
}
