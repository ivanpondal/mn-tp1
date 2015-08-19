#include "alto_horno.h"

AltoHorno::AltoHorno(const char* entrada, bool lu){
	ifstream archivoEntrada;
	archivoEntrada.open(entrada);
	cargar(archivoEntrada, lu);
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
