#include "mini_test.h"
#include "alto_horno.h"

#include <string>
#include <sstream>

void check_carga_alto_horno_instancias_una(){
	// mock
	vector<double> tempInterna(3),  tempExterna(3);
	tempInterna[0] = 1500.0;
	tempInterna[1] = 1400.0;
	tempInterna[2] = 1500.0;
	tempExterna[0] = 250.0;
	tempExterna[1] = 240.0;
	tempExterna[2] = 250.0;
	pair<vector<double>, vector<double> > temperaturas(tempInterna, tempExterna);

	// cargamos el horno
	string entrada = "tests/test_carga_1.inn";
	AltoHorno altoHorno(entrada.c_str());

	ASSERT(altoHorno.darInstancias()[0] == temperaturas);
}

void check_carga_alto_horno_instancias_dos(){
	// mock
	vector<double> tempInterna(3),  tempExterna(3);
	tempInterna[0] = 1500.0;
	tempInterna[1] = 1400.0;
	tempInterna[2] = 1500.0;
	tempExterna[0] = 250.0;
	tempExterna[1] = 240.0;
	tempExterna[2] = 250.0;
	pair<vector<double>, vector<double> > temperaturasUno(tempInterna, tempExterna);
	tempExterna[0] = 260.0;
	tempExterna[1] = 250.0;
	tempExterna[2] = 260.0;
	pair<vector<double>, vector<double> > temperaturasDos(tempInterna, tempExterna);

	// cargamos el horno
	string entrada = "tests/test_carga_2.inn";
	AltoHorno altoHorno(entrada.c_str());

	ASSERT(altoHorno.darInstancias()[0] == temperaturasUno);
	ASSERT(altoHorno.darInstancias()[1] == temperaturasDos);
}

void check_sistema_ecuaciones_LU(){
	//veamos que no tenga ninguna excepcion rara
	vector<vector<double> > A {{4, 2, 1},{20, 7, 12},{8, 13, 17}};
	vector<vector<double> > b {{1, 2, 3}};
	SistemaEcuaciones solver = SistemaEcuaciones(A, b, 3, 0);
	solver.resolverSistema(0, LU);
}

void check_isoterma(){
	string entrada = "tests/test_isoterma.inn";
	string salida = "tests/test_isoterma.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida.c_str(), GAUSS);
	altoHorno.calcularIsoterma(BINARIA);
}

// para correr un test: ./test test.in test.expected {0: EG, 1: LU}
int main(int argc, char *argv[])
{
	// si no hay argumentos corro tests unitarios, si no los de la cátedra
	if(argc == 4){
		char* entrada = argv[1];
		char* salida = argv[2];
		TipoResolucion tipo = argv[3][0] == '0' ? GAUSS : LU; // enum definido en sistema_ecuaciones.h
		AltoHorno altoHorno(entrada);
		altoHorno.generarSoluciones(salida, tipo);
	}
	else{
		RUN_TEST(check_carga_alto_horno_instancias_una);
		RUN_TEST(check_carga_alto_horno_instancias_dos);
		RUN_TEST(check_sistema_ecuaciones_LU);
		RUN_TEST(check_isoterma);
	}
	return 0;
}
