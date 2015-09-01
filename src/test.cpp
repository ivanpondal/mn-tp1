#include "mini_test.h"
#include "alto_horno.h"
#include "sistema_ecuaciones.h"
#include "utils.cpp"

#include <string>
#include <sstream>
#include <math.h>
#include <ctime>
#include <chrono>

// ----------------------------------------------------------------------------
static chrono::time_point<chrono::high_resolution_clock> start_time;

// tests generales para evaluar las funciones

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

void check_isoterma_binaria(){
	string entrada = "tests/test_isoterma.inn";
	string salida_sistema = "tests/test_isoterma.out";
	string salida_isoterma = "tests/test_isoterma_binaria.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), GAUSS);
	altoHorno.escribirIsoterma(salida_isoterma.c_str(), BINARIA);
}

void check_isoterma_avg(){
	string entrada = "tests/test_isoterma.inn";
	string salida_sistema = "tests/test_isoterma.out";
	string salida_isoterma = "tests/test_isoterma_avg.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), GAUSS);
	altoHorno.escribirIsoterma(salida_isoterma.c_str(), AVG);
}

void check_isoterma_linear_fit(){
	string entrada = "tests/test_isoterma.inn";
	string salida_sistema = "tests/test_isoterma.out";
	string salida_isoterma = "tests/test_isoterma_linear_fit.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), GAUSS);
	altoHorno.escribirIsoterma(salida_isoterma.c_str(), LINEAR_FIT);
}

void check_numero_condicion_1(){
	string entrada = "tests/test_numero_condicion_1.inn";
	string salida_sistema = "/dev/null";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), GAUSS);
	double numero_condicion = altoHorno.calcularNumeroCondicion();
	cout << endl;
	cout << "\t" << numero_condicion << endl;
}

void check_numero_condicion_2(){
	// este test duplica la discretizacion en check_numero_condicion_1
	string entrada = "tests/test_numero_condicion_2.inn";
	string salida_sistema = "/dev/null";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), GAUSS);
	double numero_condicion = altoHorno.calcularNumeroCondicion();
	cout << endl;
	cout << "\t" << numero_condicion << endl;
}

void check_numero_condicion_3(){
	// este test triplica la discretizacion en check_numero_condicion_1
	string entrada = "tests/test_numero_condicion_3.inn";
	string salida_sistema = "/dev/null";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), GAUSS);
	double numero_condicion = altoHorno.calcularNumeroCondicion();
	cout << endl;
	cout << "\t" << numero_condicion << endl;
}

void check_numero_condicion_4(){
	// este test triplica la discretizacion en check_numero_condicion_1, pero ademas triplica el espacio entre r_i y r_e
	string entrada = "tests/test_numero_condicion_4.inn";
	string salida_sistema = "/dev/null";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), GAUSS);
	double numero_condicion = altoHorno.calcularNumeroCondicion();
	cout << endl;
	cout << "\t" << numero_condicion << endl;
}

void check_calidad_isoterma(){
	string entrada = "tests/test_isoterma.inn";
	string salida_sistema = "tests/test_isoterma.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	vector<double> iso_avg = altoHorno.calcularIsoterma(AVG);
	vector<double> iso_linear_fit = altoHorno.calcularIsoterma(LINEAR_FIT);
	vector<double> iso_binaria = altoHorno.calcularIsoterma(BINARIA);
}

void check_evaluar_estructura_simple(){
	string entrada = "tests/test_isoterma.inn";
	string salida_sistema = "/dev/null";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	vector<double> iso_avg = altoHorno.calcularIsoterma(AVG);
	vector<double> iso_linear_fit = altoHorno.calcularIsoterma(LINEAR_FIT);
	vector<double> iso_binaria = altoHorno.calcularIsoterma(BINARIA);

	cout << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.25, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.25, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.25, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.15, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.15, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.15, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.05, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.05, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.05, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.01 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.01, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.01 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.01, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.01 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.01, SIMPLE) << endl;
}

void check_evaluar_estructura_promedio(){
	string entrada = "tests/test_isoterma.inn";
	string salida_sistema = "/dev/null";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	vector<double> iso_avg = altoHorno.calcularIsoterma(AVG);
	vector<double> iso_linear_fit = altoHorno.calcularIsoterma(LINEAR_FIT);
	vector<double> iso_binaria = altoHorno.calcularIsoterma(BINARIA);

	cout << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.25, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.25, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.25, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.15, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.15, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.15, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.05, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.05, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.05, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.01 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.01, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.01 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.01, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.01 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.01, PROM) << endl;
}

// ----------------------------------------------------------------------------
// tests de datos para la experimentacion del informe

void exp_discretizacion_horno_plomo_1_numero_condicion(){
	string entrada = "tests/test_horno_plomo1.inn";
	AltoHorno altoHorno(entrada.c_str());
	cout << endl;
	cout << "\tNumero condicion Horno Plomo 1: " << altoHorno.calcularNumeroCondicion() << endl;
}

void exp_discretizacion_horno_zinc_1_numero_condicion(){
	string entrada = "tests/test_horno_zinc1.inn";
	AltoHorno altoHorno(entrada.c_str());
	cout << endl;
	cout << "\tNumero condicion Horno Zinc 1: " << altoHorno.calcularNumeroCondicion() << endl;
}

void exp_discretizacion_horno_hierro_1_numero_condicion(){
	string entrada = "tests/test_horno_hierro1.inn";
	AltoHorno altoHorno(entrada.c_str());
	cout << endl;
	cout << "\tNumero condicion Horno Hierro 1: " << altoHorno.calcularNumeroCondicion() << endl;
}

void exp_discretizacion_horno_plomo_2_numero_condicion(){
	string entrada = "tests/test_horno_plomo2.inn";
	AltoHorno altoHorno(entrada.c_str());
	cout << endl;
	cout << "\tNumero condicion Horno Plomo 2: " << altoHorno.calcularNumeroCondicion() << endl;
}

void exp_discretizacion_horno_zinc_2_numero_condicion(){
	string entrada = "tests/test_horno_zinc2.inn";
	AltoHorno altoHorno(entrada.c_str());
	cout << endl;
	cout << "\tNumero condicion Horno Zinc 2: " << altoHorno.calcularNumeroCondicion() << endl;
}

void exp_discretizacion_horno_hierro_2_numero_condicion(){
	string entrada = "tests/test_horno_hierro2.inn";
	AltoHorno altoHorno(entrada.c_str());
	cout << endl;
	cout << "\tNumero condicion Horno Hierro 2: " << altoHorno.calcularNumeroCondicion() << endl;
}

void exp_calidad_solucion_horno_plomo_1() {
	string entrada = "tests/test_horno_plomo1.inn";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones("/dev/null", GAUSS);

	vector<double> X =  altoHorno.darSoluciones()[0];
	SistemaEcuaciones sistema =  altoHorno.darSistema();
	vector<vector<double> > A = sistema.darMatriz();
	vector<double> b = sistema.darInstancias()[0];
	/*cout << endl << "b = ";
	for (int i = 0; i < (int)b.size(); ++i) {
		cout << b[i] << " ";
	}*/

	// traspongo X para poder multiplicarlo por A
	vector<vector<double> > aux(X.size());
	for (int i = 0; i < (int)X.size(); ++i) {
		vector<double> elem;
		elem.push_back(X[i]);
		aux[i] = (elem);
	}
	vector<vector<double> > b_prime = Utils::multiply(A, aux);
	/*cout << endl << "b' = ";
	for (int i = 0; i < (int)b_prime.size(); ++i) {
		cout << setprecision(5) << round(b_prime[i][0]) << " ";
	}*/

	// encuentro la mayor diferencia entre los dos
	double max = -1;
	for (int i = 0; i < (int)b.size(); ++i) {
		double aux = fabs(b[i] - b_prime[i][0]);
		if(max < aux) {
			max = aux;
		}
	}

	cout << endl;
	cout << "\tLa maxima diferencia encontrada entre dos elementos de b y b' es: " << max << endl;
}

void exp_calidad_solucion_horno_zinc_1() {
	string entrada = "tests/test_horno_zinc1.inn";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones("/dev/null", GAUSS);

	vector<double> X =  altoHorno.darSoluciones()[0];
	SistemaEcuaciones sistema =  altoHorno.darSistema();
	vector<vector<double> > A = sistema.darMatriz();
	vector<double> b = sistema.darInstancias()[0];
	/*cout << endl << "b = ";
	for (int i = 0; i < (int)b.size(); ++i) {
		cout << b[i] << " ";
	}*/

	// traspongo X para poder multiplicarlo por A
	vector<vector<double> > aux(X.size());
	for (int i = 0; i < (int)X.size(); ++i) {
		vector<double> elem;
		elem.push_back(X[i]);
		aux[i] = (elem);
	}
	vector<vector<double> > b_prime = Utils::multiply(A, aux);
	/*cout << endl << "b' = ";
	for (int i = 0; i < (int)b_prime.size(); ++i) {
		cout << setprecision(5) << round(b_prime[i][0]) << " ";
	}*/

	// encuentro la mayor diferencia entre los dos
	double max = -1;
	for (int i = 0; i < (int)b.size(); ++i) {
		double aux = fabs(b[i] - b_prime[i][0]);
		if(max < aux) {
			max = aux;
		}
	}

	cout << endl;
	cout << "\tLa maxima diferencia encontrada entre dos elementos de b y b' es: " << max << endl;
}

void exp_calidad_solucion_horno_hierro_1() {
	string entrada = "tests/test_horno_hierro1.inn";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones("/dev/null", GAUSS);

	vector<double> X =  altoHorno.darSoluciones()[0];
	SistemaEcuaciones sistema =  altoHorno.darSistema();
	vector<vector<double> > A = sistema.darMatriz();
	vector<double> b = sistema.darInstancias()[0];
	/*cout << endl << "b = ";
	for (int i = 0; i < (int)b.size(); ++i) {
		cout << b[i] << " ";
	}*/

	// traspongo X para poder multiplicarlo por A
	vector<vector<double> > aux(X.size());
	for (int i = 0; i < (int)X.size(); ++i) {
		vector<double> elem;
		elem.push_back(X[i]);
		aux[i] = (elem);
	}
	vector<vector<double> > b_prime = Utils::multiply(A, aux);
	/*cout << endl << "b' = ";
	for (int i = 0; i < (int)b_prime.size(); ++i) {
		cout << setprecision(5) << round(b_prime[i][0]) << " ";
	}*/

	// encuentro la mayor diferencia entre los dos
	double max = -1;
	for (int i = 0; i < (int)b.size(); ++i) {
		double aux = fabs(b[i] - b_prime[i][0]);
		if(max < aux) {
			max = aux;
		}
	}

	cout << endl;
	cout << "\tLa maxima diferencia encontrada entre dos elementos de b y b' es: " << max << endl;
}

void exp_calidad_solucion_horno_plomo_2() {
	string entrada = "tests/test_horno_plomo2.inn";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones("/dev/null", GAUSS);

	vector<double> X =  altoHorno.darSoluciones()[0];
	SistemaEcuaciones sistema =  altoHorno.darSistema();
	vector<vector<double> > A = sistema.darMatriz();
	vector<double> b = sistema.darInstancias()[0];
	/*cout << endl << "b = ";
	for (int i = 0; i < (int)b.size(); ++i) {
		cout << b[i] << " ";
	}*/

	// traspongo X para poder multiplicarlo por A
	vector<vector<double> > aux(X.size());
	for (int i = 0; i < (int)X.size(); ++i) {
		vector<double> elem;
		elem.push_back(X[i]);
		aux[i] = (elem);
	}
	vector<vector<double> > b_prime = Utils::multiply(A, aux);
	/*cout << endl << "b' = ";
	for (int i = 0; i < (int)b_prime.size(); ++i) {
		cout << setprecision(5) << round(b_prime[i][0]) << " ";
	}*/

	// encuentro la mayor diferencia entre los dos
	double max = -1;
	for (int i = 0; i < (int)b.size(); ++i) {
		double aux = fabs(b[i] - b_prime[i][0]);
		if(max < aux) {
			max = aux;
		}
	}

	cout << endl;
	cout << "\tLa maxima diferencia encontrada entre dos elementos de b y b' es: " << max << endl;
}

void exp_calidad_solucion_horno_zinc_2() {
	string entrada = "tests/test_horno_zinc2.inn";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones("/dev/null", GAUSS);

	vector<double> X =  altoHorno.darSoluciones()[0];
	SistemaEcuaciones sistema =  altoHorno.darSistema();
	vector<vector<double> > A = sistema.darMatriz();
	vector<double> b = sistema.darInstancias()[0];
	/*cout << endl << "b = ";
	for (int i = 0; i < (int)b.size(); ++i) {
		cout << b[i] << " ";
	}*/

	// traspongo X para poder multiplicarlo por A
	vector<vector<double> > aux(X.size());
	for (int i = 0; i < (int)X.size(); ++i) {
		vector<double> elem;
		elem.push_back(X[i]);
		aux[i] = (elem);
	}
	vector<vector<double> > b_prime = Utils::multiply(A, aux);
	/*cout << endl << "b' = ";
	for (int i = 0; i < (int)b_prime.size(); ++i) {
		cout << setprecision(5) << round(b_prime[i][0]) << " ";
	}*/

	// encuentro la mayor diferencia entre los dos
	double max = -1;
	for (int i = 0; i < (int)b.size(); ++i) {
		double aux = fabs(b[i] - b_prime[i][0]);
		if(max < aux) {
			max = aux;
		}
	}

	cout << endl;
	cout << "\tLa maxima diferencia encontrada entre dos elementos de b y b' es: " << max << endl;
}

void exp_calidad_solucion_horno_hierro_2() {
	string entrada = "tests/test_horno_hierro2.inn";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones("/dev/null", GAUSS);

	vector<double> X =  altoHorno.darSoluciones()[0];
	SistemaEcuaciones sistema =  altoHorno.darSistema();
	vector<vector<double> > A = sistema.darMatriz();
	vector<double> b = sistema.darInstancias()[0];
	/*cout << endl << "b = ";
	for (int i = 0; i < (int)b.size(); ++i) {
		cout << b[i] << " ";
	}*/

	// traspongo X para poder multiplicarlo por A
	vector<vector<double> > aux(X.size());
	for (int i = 0; i < (int)X.size(); ++i) {
		vector<double> elem;
		elem.push_back(X[i]);
		aux[i] = (elem);
	}
	vector<vector<double> > b_prime = Utils::multiply(A, aux);
	/*cout << endl << "b' = ";
	for (int i = 0; i < (int)b_prime.size(); ++i) {
		cout << setprecision(5) << round(b_prime[i][0]) << " ";
	}*/

	// encuentro la mayor diferencia entre los dos
	double max = -1;
	for (int i = 0; i < (int)b.size(); ++i) {
		double aux = fabs(b[i] - b_prime[i][0]);
		if(max < aux) {
			max = aux;
		}
	}

	cout << endl;
	cout << "\tLa maxima diferencia encontrada entre dos elementos de b y b' es: " << max << endl;
}

void exp_isoterma_horno_plomo_1(){
	string entrada = "tests/test_horno_plomo1.inn";
	string salida_sistema = "tests/test_horno_plomo1.out";
	string salida_isoterma_avg = "tests/test_isoterma_horno_plomo_1_avg.out";
	string salida_isoterma_linear_fit = "tests/test_isoterma_horno_plomo_1_linear_fit.out";
	string salida_isoterma_binaria = "tests/test_isoterma_horno_plomo_1_binaria.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	altoHorno.escribirIsoterma(salida_isoterma_avg.c_str(), AVG);
	altoHorno.escribirIsoterma(salida_isoterma_linear_fit.c_str(), LINEAR_FIT);
	altoHorno.escribirIsoterma(salida_isoterma_binaria.c_str(), BINARIA);

	// generar imagenes:
	cout << endl << "\tGenerando imagenes..." << endl;
	char command[500];
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_avg.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_linear_fit.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_binaria.c_str());
	if(system(command)) { cout << "System failed" << endl; };
}

void exp_isoterma_horno_zinc_1(){
	string entrada = "tests/test_horno_zinc1.inn";
	string salida_sistema = "tests/test_horno_zinc1.out";
	string salida_isoterma_avg = "tests/test_isoterma_horno_zinc_1_avg.out";
	string salida_isoterma_linear_fit = "tests/test_isoterma_horno_zinc_1_linear_fit.out";
	string salida_isoterma_binaria = "tests/test_isoterma_horno_zinc_1_binaria.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	altoHorno.escribirIsoterma(salida_isoterma_avg.c_str(), AVG);
	altoHorno.escribirIsoterma(salida_isoterma_linear_fit.c_str(), LINEAR_FIT);
	altoHorno.escribirIsoterma(salida_isoterma_binaria.c_str(), BINARIA);

	// generar imagenes:
	cout << endl << "\tGenerando imagenes..." << endl;
	char command[500];
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_avg.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_linear_fit.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_binaria.c_str());
	if(system(command)) { cout << "System failed" << endl; };
}

void exp_isoterma_horno_hierro_1(){
	string entrada = "tests/test_horno_hierro1.inn";
	string salida_sistema = "tests/test_horno_hierro1.out";
	string salida_isoterma_avg = "tests/test_isoterma_horno_hierro_1_avg.out";
	string salida_isoterma_linear_fit = "tests/test_isoterma_horno_hierro_1_linear_fit.out";
	string salida_isoterma_binaria = "tests/test_isoterma_horno_hierro_1_binaria.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	altoHorno.escribirIsoterma(salida_isoterma_avg.c_str(), AVG);
	altoHorno.escribirIsoterma(salida_isoterma_linear_fit.c_str(), LINEAR_FIT);
	altoHorno.escribirIsoterma(salida_isoterma_binaria.c_str(), BINARIA);

	// generar imagenes:
	cout << endl << "\tGenerando imagenes..." << endl;
	char command[500];
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_avg.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_linear_fit.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_binaria.c_str());
	if(system(command)) { cout << "System failed" << endl; };
}

void exp_isoterma_horno_plomo_2(){
	string entrada = "tests/test_horno_plomo2.inn";
	string salida_sistema = "tests/test_horno_plomo2.out";
	string salida_isoterma_avg = "tests/test_isoterma_horno_plomo_2_avg.out";
	string salida_isoterma_linear_fit = "tests/test_isoterma_horno_plomo_2_linear_fit.out";
	string salida_isoterma_binaria = "tests/test_isoterma_horno_plomo_2_binaria.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	altoHorno.escribirIsoterma(salida_isoterma_avg.c_str(), AVG);
	altoHorno.escribirIsoterma(salida_isoterma_linear_fit.c_str(), LINEAR_FIT);
	altoHorno.escribirIsoterma(salida_isoterma_binaria.c_str(), BINARIA);

	// generar imagenes:
	cout << endl << "\tGenerando imagenes..." << endl;
	char command[500];
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_avg.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_linear_fit.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_binaria.c_str());
	if(system(command)) { cout << "System failed" << endl; };
}

void exp_isoterma_horno_zinc_2(){
	string entrada = "tests/test_horno_zinc2.inn";
	string salida_sistema = "tests/test_horno_zinc2.out";
	string salida_isoterma_avg = "tests/test_isoterma_horno_zinc_2_avg.out";
	string salida_isoterma_linear_fit = "tests/test_isoterma_horno_zinc_2_linear_fit.out";
	string salida_isoterma_binaria = "tests/test_isoterma_horno_zinc_2_binaria.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	altoHorno.escribirIsoterma(salida_isoterma_avg.c_str(), AVG);
	altoHorno.escribirIsoterma(salida_isoterma_linear_fit.c_str(), LINEAR_FIT);
	altoHorno.escribirIsoterma(salida_isoterma_binaria.c_str(), BINARIA);

	// generar imagenes:
	cout << endl << "\tGenerando imagenes..." << endl;
	char command[500];
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_avg.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_linear_fit.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_binaria.c_str());
	if(system(command)) { cout << "System failed" << endl; };
}

void exp_isoterma_horno_hierro_2(){
	string entrada = "tests/test_horno_hierro2.inn";
	string salida_sistema = "tests/test_horno_hierro2.out";
	string salida_isoterma_avg = "tests/test_isoterma_horno_hierro_2_avg.out";
	string salida_isoterma_linear_fit = "tests/test_isoterma_horno_hierro_2_linear_fit.out";
	string salida_isoterma_binaria = "tests/test_isoterma_horno_hierro_2_binaria.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	altoHorno.escribirIsoterma(salida_isoterma_avg.c_str(), AVG);
	altoHorno.escribirIsoterma(salida_isoterma_linear_fit.c_str(), LINEAR_FIT);
	altoHorno.escribirIsoterma(salida_isoterma_binaria.c_str(), BINARIA);

	// generar imagenes:
	cout << endl << "\tGenerando imagenes..." << endl;
	char command[500];
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_avg.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_linear_fit.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_binaria.c_str());
	if(system(command)) { cout << "System failed" << endl; };
}

void exp_isoterma_horno_plomo_3a(){
	string entrada = "tests/test_horno_plomo3a.inn";
	string salida_sistema = "tests/test_horno_plomo3a.out";
	string salida_isoterma_avg = "tests/test_isoterma_horno_plomo_3a_avg.out";
	string salida_isoterma_linear_fit = "tests/test_isoterma_horno_plomo_3a_linear_fit.out";
	string salida_isoterma_binaria = "tests/test_isoterma_horno_plomo_3a_binaria.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	altoHorno.escribirIsoterma(salida_isoterma_avg.c_str(), AVG);
	altoHorno.escribirIsoterma(salida_isoterma_linear_fit.c_str(), LINEAR_FIT);
	altoHorno.escribirIsoterma(salida_isoterma_binaria.c_str(), BINARIA);

	// generar imagenes:
	cout << endl << "\tGenerando imagenes..." << endl;
	char command[500];
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_avg.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_linear_fit.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_binaria.c_str());
	if(system(command)) { cout << "System failed" << endl; };

	// evaluando la peligrosidad de la estructura:
	vector<double> iso_avg = altoHorno.calcularIsoterma(AVG);
	vector<double> iso_linear_fit = altoHorno.calcularIsoterma(LINEAR_FIT);
	vector<double> iso_binaria = altoHorno.calcularIsoterma(BINARIA);

	cout << endl << "\tEvaluando estructura..." << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.25, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.25, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.25, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.15, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.15, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.15, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.10 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.10, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.10 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.10, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.10 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.10, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.05, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.05, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.05, SIMPLE) << endl;

	cout << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.25, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.25, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.25, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.15, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.15, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.15, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.10 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.10, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.10 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.10, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.10 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.10, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.05, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.05, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.05, PROM) << endl;
}

void exp_isoterma_horno_plomo_3b(){
	string entrada = "tests/test_horno_plomo3b.inn";
	string salida_sistema = "tests/test_horno_plomo3b.out";
	string salida_isoterma_avg = "tests/test_isoterma_horno_plomo_3b_avg.out";
	string salida_isoterma_linear_fit = "tests/test_isoterma_horno_plomo_3b_linear_fit.out";
	string salida_isoterma_binaria = "tests/test_isoterma_horno_plomo_3b_binaria.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	altoHorno.escribirIsoterma(salida_isoterma_avg.c_str(), AVG);
	altoHorno.escribirIsoterma(salida_isoterma_linear_fit.c_str(), LINEAR_FIT);
	altoHorno.escribirIsoterma(salida_isoterma_binaria.c_str(), BINARIA);

	// generar imagenes:
	cout << endl << "\tGenerando imagenes..." << endl;
	char command[500];
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_avg.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_linear_fit.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_binaria.c_str());
	if(system(command)) { cout << "System failed" << endl; };

	// evaluando la peligrosidad de la estructura:
	vector<double> iso_avg = altoHorno.calcularIsoterma(AVG);
	vector<double> iso_linear_fit = altoHorno.calcularIsoterma(LINEAR_FIT);
	vector<double> iso_binaria = altoHorno.calcularIsoterma(BINARIA);

	cout << endl << "\tEvaluando estructura..." << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.25, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.25, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.25, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.15, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.15, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.15, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.10 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.10, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.10 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.10, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.10 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.10, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.05, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.05, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.05, SIMPLE) << endl;

	cout << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.25, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.25, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.25, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.15, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.15, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.15, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.10 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.10, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.10 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.10, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.10 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.10, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.05, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.05, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.05, PROM) << endl;
}

void exp_isoterma_horno_plomo_3c(){
	string entrada = "tests/test_horno_plomo3c.inn";
	string salida_sistema = "tests/test_horno_plomo3c.out";
	string salida_isoterma_avg = "tests/test_isoterma_horno_plomo_3c_avg.out";
	string salida_isoterma_linear_fit = "tests/test_isoterma_horno_plomo_3c_linear_fit.out";
	string salida_isoterma_binaria = "tests/test_isoterma_horno_plomo_3c_binaria.out";
	AltoHorno altoHorno(entrada.c_str());
	altoHorno.generarSoluciones(salida_sistema.c_str(), LU);
	altoHorno.escribirIsoterma(salida_isoterma_avg.c_str(), AVG);
	altoHorno.escribirIsoterma(salida_isoterma_linear_fit.c_str(), LINEAR_FIT);
	altoHorno.escribirIsoterma(salida_isoterma_binaria.c_str(), BINARIA);

	// generar imagenes:
	cout << endl << "\tGenerando imagenes..." << endl;
	char command[500];
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_avg.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_linear_fit.c_str());
	if(system(command)) { cout << "System failed" << endl; };
	sprintf(command, "octave --path tools/ --eval 'horno %s %s %s' >> /dev/null", entrada.c_str(), salida_sistema.c_str(), salida_isoterma_binaria.c_str());
	if(system(command)) { cout << "System failed" << endl; };

	// evaluando la peligrosidad de la estructura:
	vector<double> iso_avg = altoHorno.calcularIsoterma(AVG);
	vector<double> iso_linear_fit = altoHorno.calcularIsoterma(LINEAR_FIT);
	vector<double> iso_binaria = altoHorno.calcularIsoterma(BINARIA);

	cout << endl << "\tEvaluando estructura..." << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.25, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.25, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.25 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.25, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.15, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.15, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.15 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.15, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.10 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.10, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.10 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.10, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.10 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.10, SIMPLE) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.05, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.05, SIMPLE) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (SIMPLE, epsilon: "<< 0.05 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.05, SIMPLE) << endl;

	cout << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.25, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.25, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.25 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.25, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.15, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.15, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.15 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.15, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.10 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.10, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.10 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.10, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.10 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.10, PROM) << endl;

	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_avg): " << altoHorno.evaluarEstructura(iso_avg, 0.05, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_linear_fit): " << altoHorno.evaluarEstructura(iso_linear_fit, 0.05, PROM) << endl;
	cout << boolalpha << "\tLa estructura esta en peligro (PROM, epsilon: "<< 0.05 <<", iso_binaria): " << altoHorno.evaluarEstructura(iso_binaria, 0.05, PROM) << endl;
}

void start_timer() {
    start_time = chrono::high_resolution_clock::now();
}

double stop_timer() {
    chrono::time_point<chrono::high_resolution_clock> end_time = chrono::high_resolution_clock::now();
    return double(chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count());
}


void exp_temporal() {
	// Archivo donde guardo resultados
	FILE* salida = fopen("exp/salida.txt","w+");
	//fprintf(salida, "n*m t_G t_LU\n");
	// Valores del horno de hierro
	int ri = 11;
	int re = 15;
	int iso = 500;
	int ninst = 1;
	int Ti = 1538;
	int Te = 20;
	// Genero instancias del problema con valores del horno de hierro
	int n = 3;
	int m = 3;
	// Valores de la experimentacion, fijando el n
	int limite_m = 60;
	int muestras = 30;
	// Guardo tiempos
	vector <double> tiempos_gauss(limite_m, 0);
	vector <double> tiempos_LU(limite_m, 0);
	for (int i = 0; i < limite_m; i++) {
		FILE* entrada = fopen("exp/entrada.in","w+");
		//FILE* prueba = fopen("exp/prueba.in","w+");
		fprintf(entrada, "%d %d %d %d %d %d \n", ri, re, m+i, n, iso, ninst);
		for (int j = 0; j < n; j++) {
			fprintf(entrada, "%d %s", Ti, " ");
		}

		for (int j = 0; j < n; j++) {
			fprintf(entrada, "%d %s", Te, " ");
		}
		fclose(entrada);
		//fclose(prueba);
		// Solucion para Gauss
		for (int muestra = 0; muestra < muestras; muestra++) {
			double tiempo;
			AltoHorno instancia("exp/entrada.in");
			start_timer();
			instancia.resolverSistema(GAUSS);
			tiempo = stop_timer();
			tiempos_gauss[i] += tiempo;
		}
		// Solucion LU
		for (int muestra = 0; muestra < muestras; muestra++) {
			double tiempo1;
			AltoHorno instancia("exp/entrada.in");
			start_timer();
			instancia.resolverSistema(LU);
			tiempo1 = stop_timer();
			tiempos_LU[i] += tiempo1;
		}
	}
	for (unsigned int i = 0; i < tiempos_gauss.size(); i++) {
		fprintf(salida, "%d %.0f %.0f\n", n*(i+m), tiempos_gauss[i]/muestras, tiempos_LU[i]/muestras);
	}
	fclose(salida);
}

// para correr un test: ./test test.in test.expected {0: EG, 1: LU}
int main(int argc, char *argv[])
{
	// si no hay argumentos corro tests unitarios, si no los de la cátedra
	exp_temporal();
	if(argc == 4){
		/*char* entrada = argv[1];
		char* salida = argv[2];
		TipoResolucion tipo = argv[3][0] == '0' ? GAUSS : LU; // enum definido en sistema_ecuaciones.h
		AltoHorno altoHorno(entrada);
		altoHorno.generarSoluciones(salida, tipo);*/
	}
	else{
		// tests generales para evaluar las funciones
		/*RUN_TEST(check_carga_alto_horno_instancias_una);
		RUN_TEST(check_carga_alto_horno_instancias_dos);
		RUN_TEST(check_sistema_ecuaciones_LU);
		RUN_TEST(check_isoterma_binaria);
		RUN_TEST(check_isoterma_avg);
		RUN_TEST(check_isoterma_linear_fit);
		RUN_TEST(check_numero_condicion_1);
		RUN_TEST(check_evaluar_estructura_simple);
		RUN_TEST(check_evaluar_estructura_promedio);*/
		// tests pesados que tardan un poco más:
		//RUN_TEST(check_numero_condicion_2);
		//RUN_TEST(check_numero_condicion_3);
		//RUN_TEST(check_numero_condicion_4);
		//RUN_TEST(check_calidad_isoterma);

		// tests de datos para la experimentacion del informe
		// discretización:
		/*RUN_TEST(exp_discretizacion_horno_plomo_1_numero_condicion);
		RUN_TEST(exp_discretizacion_horno_zinc_1_numero_condicion);
		RUN_TEST(exp_discretizacion_horno_hierro_1_numero_condicion);*/
		// tests pesados que tardan un poco más:
		// RUN_TEST(exp_discretizacion_horno_plomo_2_numero_condicion);
		// RUN_TEST(exp_discretizacion_horno_zinc_2_numero_condicion);
		// RUN_TEST(exp_discretizacion_horno_hierro_2_numero_condicion);
		// calidad de la solucion:
		/*RUN_TEST(exp_calidad_solucion_horno_plomo_1);
		RUN_TEST(exp_calidad_solucion_horno_zinc_1);
		RUN_TEST(exp_calidad_solucion_horno_hierro_1);
		RUN_TEST(exp_calidad_solucion_horno_plomo_2);
		RUN_TEST(exp_calidad_solucion_horno_zinc_2);
		RUN_TEST(exp_calidad_solucion_horno_hierro_2);*/

		// isotermas:
		/*RUN_TEST(exp_isoterma_horno_plomo_1);
		RUN_TEST(exp_isoterma_horno_zinc_1);
		RUN_TEST(exp_isoterma_horno_hierro_1);*/
		// tests pesados que tardan un poco más:
		// RUN_TEST(exp_isoterma_horno_plomo_2);
		// RUN_TEST(exp_isoterma_horno_zinc_2);
		// RUN_TEST(exp_isoterma_horno_hierro_2);

		// proximidad de la isoterma:
		// RUN_TEST(exp_isoterma_horno_plomo_3a);
		// RUN_TEST(exp_isoterma_horno_plomo_3b);
		// RUN_TEST(exp_isoterma_horno_plomo_3c);

		// script importante:
		// octave --path tools/ --eval 'horno tests/test_horno_hierro1.inn tests/test_horno_hierro1.out tests/test_isoterma_horno_hierro_1_binaria.out'
	}
	return 0;
}
