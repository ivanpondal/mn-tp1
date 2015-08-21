#include <sistema_ecuaciones.h>

SistemaEcuaciones::SistemaEcuaciones(){
	this->dimMatriz = 0;
}

SistemaEcuaciones::SistemaEcuaciones(vector<vector<double> > A, vector<vector<double> > instB, int dimMatriz){
	this->A = A;
	this->instB = instB;
	this->dimMatriz = dimMatriz;
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
		eliminacionGaussiana(mA, b, this->dimMatriz);
		return resolverTriangular(mA, b, this->dimMatriz);
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
	cout<<endl;
	for(int j = 0; j < dimMatriz; j++){
		cout<<"[";
		for(int k = 0; k < dimMatriz; k++){
			cout<<" "<<mA[j][k];
		}
		cout<<" | "<<b[j]<<" ]"<<endl;
	}
}
