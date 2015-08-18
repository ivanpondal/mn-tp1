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
		int tmp;
		vector<int> tempInterior(this->cantAngulos), tempExterior(this->cantAngulos);
		for(int j = 0; j < this->cantAngulos; j++){
			entrada >> tmp;
			tempInterior[j] = tmp;
		}
		for(int j = 0; j < this->cantAngulos; j++){
			entrada >> tmp;
			tempExterior[j] = tmp;
		}
		this->instancias[i] = pair<vector<int>, vector<int> >(tempInterior, tempExterior);
	}
	usaLU = lu;
}

int AltoHorno::darRadioInterior(){
	return this->radioInterior;
}

int AltoHorno::darRadioExterior(){
	return this->radioExterior;
}

int AltoHorno::darCantParticiones(){
	return this->cantParticiones;
}

int AltoHorno::darCantAngulos(){
	return this->cantAngulos;
}

int AltoHorno::darIsoterma(){
	return this->isoterma;
}

int AltoHorno::darCantInstancias(){
	return this->cantInstancias;
}

map<int, pair<vector<int>, vector<int> > > AltoHorno::darInstancias(){
	return this->instancias;
}
