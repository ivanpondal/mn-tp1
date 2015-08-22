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

vector<double> SistemaEcuaciones::resolverSistema(int instancia, TipoResolucion tipo){
	// copio por si luego quisiera usar otro método
	vector<vector<double> > mA = this->A;
	vector<double> b = this->instB[instancia];

	if (tipo == LU) {
		//calculo la factorizacion LU de A y la guardo en this->factorizacionLU (si no la habia calculado previamente)
		if (factorizacionLU.size()==0){
			factorizarLU(this->A);
		}
		//ahora como L*U*x = A*x = b, sea U*x = y => L*y = b
		vector<double> y = resolverTriangularInferiorLU(this->factorizacionLU, b, this->dimMatriz);
		for (int i = 0; i<y.size(); ++i) cout <<y[i]<<endl;
		//ya obtuve 'y', ahora hallo 'x' resolviendo U*x = y
		vector<double> x = resolverTriangularSuperior(this->factorizacionLU, y, this->dimMatriz);
		for (int i = 0; i<x.size(); ++i) cout <<x[i]<<endl;
		return x;
	} else {
		if (BANDA) {
			eliminacionGaussianaBanda(mA, b, this->dimMatriz, this->cantAngulos);
			return resolverTriangularSuperior(mA, b, this->dimMatriz);	
		} else {
			eliminacionGaussiana(mA, b, this->dimMatriz);
			return resolverTriangularSuperior(mA, b, this->dimMatriz);	
		}
	}
}

vector<double> SistemaEcuaciones::resolverTriangularSuperior(const vector<vector<double> > &U, const vector<double> &b, int n){
	vector<double> x(n, 0);
	for(int i = n-1; i >= 0; --i) {
		x[i] = b[i];
		for(int j = i+1; j < n; j++) {
			x[i] -= U[i][j] * x[j];
		}
		x[i] /= U[i][i];
	}
	return x;
}

vector<double> SistemaEcuaciones::resolverTriangularInferior(const vector<vector<double> > &U, const vector<double> &b, int n){
	vector<double> x(n, 0);
	for(int i = 0; i < n; ++i) {
		x[i] = b[i];
		for (int j = 0; j < i; ++j) {
			x[i] -= x[i] * U[i][j];
		}
		x[i] /= U[i][i];
	}
	return x;
}

vector<double> SistemaEcuaciones::resolverTriangularInferiorLU(const vector<vector<double> > &U, const vector<double> &b, int n){
	vector<double> x(n, 0);
	for(int i = 0; i < n; ++i) {
		x[i] = b[i];
		for (int j = 0; j < i; ++j) {
			x[i] -= x[i] * U[i][j];
		}
		// la diagonal es 1, asi que no hay que dividir por nada
	}
	return x;
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

void SistemaEcuaciones::factorizarLU(const vector<vector<double> > &A){
	//obtener L y U, va a ser lo que quede en A
	this->factorizacionLU = A;
	for (int i = 0; i < dimMatriz-1; ++i) {//fila
		for (int j = i+1; j < dimMatriz; ++j) {//fila
			double c = factorizacionLU[j][i]/factorizacionLU[i][i];
			factorizacionLU[j][i] = c;
			for (int k = i+1; k < dimMatriz; ++k) {//columnas
				factorizacionLU[j][k]-= c*factorizacionLU[i][k];
			}
		}
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

void SistemaEcuaciones::imprimirLU(){
	cout << endl;
	for (int i = 0; i < dimMatriz; ++i){
		for (int j = 0; j < dimMatriz; ++j){
			if (i==j) cout << "1 ";
			if (j<i) cout << this->factorizacionLU[i][j] << " ";
			if (j>i) cout << "0 ";
		}
		cout << endl;
	}
	cout << endl;
	for (int i = 0; i < dimMatriz; ++i){
		for (int j = 0; j < dimMatriz; ++j){
			if (j>=i) cout << this->factorizacionLU[i][j] << " ";
			if (j<i) cout << "0 ";
		}
		cout << endl;
	}
}
