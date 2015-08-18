#include "mini_test.h"
#include "alto_horno.h"

#include <string>
#include <sstream>

void check_carga_alto_horno_instancias_una(){
	// mock
	vector<int> tempInterna(3),  tempExterna(3);
	tempInterna[0] = 1500;
	tempInterna[1] = 1400;
	tempInterna[2] = 1500;
	tempExterna[0] = 250;
	tempExterna[1] = 240;
	tempExterna[2] = 250;
	pair<vector<int>, vector<int> > temperaturas(tempInterna, tempExterna);

	// cargamos el horno
	string entrada = "tests/test_carga_1.in";
	AltoHorno altoHorno(entrada.c_str(), false);

	ASSERT_EQ(altoHorno.darRadioInterior(), 10);
	ASSERT_EQ(altoHorno.darRadioExterior(), 100);
	ASSERT_EQ(altoHorno.darCantParticiones(), 3);
	ASSERT_EQ(altoHorno.darCantAngulos(), 3);
	ASSERT_EQ(altoHorno.darIsoterma(), 500);
	ASSERT_EQ(altoHorno.darCantInstancias(), 1);
	ASSERT(altoHorno.darInstancias()[0] == temperaturas);
}

void check_carga_alto_horno_instancias_dos(){
	// mock
	vector<int> tempInterna(3),  tempExterna(3);
	tempInterna[0] = 1500;
	tempInterna[1] = 1400;
	tempInterna[2] = 1500;
	tempExterna[0] = 250;
	tempExterna[1] = 240;
	tempExterna[2] = 250;
	pair<vector<int>, vector<int> > temperaturasUno(tempInterna, tempExterna);
	tempExterna[0] = 260;
	tempExterna[1] = 250;
	tempExterna[2] = 260;
	pair<vector<int>, vector<int> > temperaturasDos(tempInterna, tempExterna);

	// cargamos el horno
	string entrada = "tests/test_carga_2.in";
	AltoHorno altoHorno(entrada.c_str(), false);

	ASSERT_EQ(altoHorno.darRadioInterior(), 10);
	ASSERT_EQ(altoHorno.darRadioExterior(), 100);
	ASSERT_EQ(altoHorno.darCantParticiones(), 3);
	ASSERT_EQ(altoHorno.darCantAngulos(), 3);
	ASSERT_EQ(altoHorno.darIsoterma(), 500);
	ASSERT_EQ(altoHorno.darCantInstancias(), 2);
	ASSERT(altoHorno.darInstancias()[0] == temperaturasUno);
	ASSERT(altoHorno.darInstancias()[1] == temperaturasDos);
}

int main(int argc, char **argv)
{
	RUN_TEST(check_carga_alto_horno_instancias_una);
	RUN_TEST(check_carga_alto_horno_instancias_dos);
	return 0;
}
