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
	AltoHorno altoHorno(entrada.c_str(), false);

	ASSERT_EQ(altoHorno.darRadioInterior(), 10.0);
	ASSERT_EQ(altoHorno.darRadioExterior(), 100.0);
	ASSERT_EQ(altoHorno.darCantParticiones(), 3);
	ASSERT_EQ(altoHorno.darCantAngulos(), 3);
	ASSERT_EQ(altoHorno.darIsoterma(), 500.0);
	ASSERT_EQ(altoHorno.darCantInstancias(), 1);
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
	AltoHorno altoHorno(entrada.c_str(), false);

	ASSERT_EQ(altoHorno.darRadioInterior(), 10.0);
	ASSERT_EQ(altoHorno.darRadioExterior(), 100.0);
	ASSERT_EQ(altoHorno.darCantParticiones(), 3);
	ASSERT_EQ(altoHorno.darCantAngulos(), 3);
	ASSERT_EQ(altoHorno.darIsoterma(), 500.0);
	ASSERT_EQ(altoHorno.darCantInstancias(), 2);
	ASSERT(altoHorno.darInstancias()[0] == temperaturasUno);
	ASSERT(altoHorno.darInstancias()[1] == temperaturasDos);
}


// para correr un test: ./test test.in test.expected {0: EG, 1: LU}
int main(int argc, char *argv[])
{
	// si no hay argumentos corro tests unitarios, si no los de la cátedra
	if(argc == 4){
		char* entrada = argv[1];
		char* salida = argv[2];
		bool usaLu = argv[3][0] == '1';
		AltoHorno altoHorno(entrada, usaLu);
		altoHorno.generarSoluciones(salida);
	}
	else{
		RUN_TEST(check_carga_alto_horno_instancias_una);
		RUN_TEST(check_carga_alto_horno_instancias_dos);
	}
	return 0;
}
