/*
Volumen de un sólido de revolución por el método de Monte Carlo
Partimos de la función de la integral de Gauss, 𝑓(𝑥) = 𝑒−𝑥2, que cumple −∞∞∫ 𝑒−𝑥2 𝑑𝑥 = √𝜋.  Al girar su gráfica 360° alrededor de 
un eje se obtiene un sólido de revolución. El objetivo es calcular su volumen en C por el método de Monte Carlo, generando puntos 
aleatorios (x, y, z) en el espacio, en dos casos: girando la curva alrededor del eje x y alrededor del eje y.

Figura 1. En naranja, la curva y = e^(−x²); en azul, el sólido que genera al girar. La caja discontinua es la región donde se
generan los puntos aleatorios.

(a) Giro alrededor del eje x. Cada corte perpendicular al eje x es un círculo de radio 𝑟 = 𝑒−𝑥2. Un punto (x, y, z) está dentro 
del sólido si su distancia al eje x es menor que ese radio:
𝑦² + 𝑧² ≤ 𝑒(−2𝑥²)

(b) Giro alrededor del eje y. El sólido es una campana de altura 1 cuya superficie es 𝑦 = 𝑒−(𝑥²+𝑧²). Un punto (x, y, z), con y ≥ 0, 
está dentro si queda por debajo de la superficie:
𝑦 ≤ 𝑒−(𝑥²+𝑧²)


Resultado analítico
(a) Eje x. Sumando discos de radio 𝑒−𝑥² y usando∫ 𝑒−𝑎𝑥2 𝑑𝑥 = √𝜋/𝑎:
𝑉 = 𝜋 −∞∞∫ 𝑒−2𝑥²
𝑑𝑥 = 𝜋 · √(𝜋/2) ≈ 3,9374

(b) Eje y. Sumando capas cilíndricas de radio x y altura 𝑒−𝑥2:
𝑉 = ∫ 0∞∫2𝜋𝑥𝑒(−𝑥²)𝑑𝑥 = 𝜋 ≈ 3,1416

En el caso (b) se integra solo desde 0, porque las mitades x < 0 y x > 0 de la curva barren el mismo sólido al girar alrededor 
del eje y.

Pasos para programarlo en C:

1. Caja de muestreo. La función no se anula nunca, así que hay que cortar el dominio en |x| ≤ L. Toma L = 4, para este valor la 
función es práticamente nula (𝑒−16 ≈ 10⁻⁷). Para el caso (a), la caja es [−L, L] × [−1, 1] × [−1, 1]; para el (b), [−L, L] × 
[0, 1] × [−L, L] en (x, y, z). Calcula el volumen de cada caja.

2. Números aleatorios. Escribe una función double uniforme(double a, double b) que devuelva un número uniforme en 
[a, b) usando rand() y RAND_MAX. Inicializa el generador una sola vez al principio del programa con srand().

3. Conteo. Para cada caso, escribe una función que genere N puntos (x, y, z) dentro de su caja y cuente cuántos cumplen la 
condición de estar dentro del sólido.

4. Estimación. El volumen es el volumen de la caja multiplicado por la fracción de puntos que han caído dentro: 
V ≈ V_caja · (dentro / N).

5. Convergencia. Repite el cálculo para N = 10², 10³, …, 10⁷, calcula el error relativo respecto al
resultado analítico y guarda los resultados en un fichero. Representa el error frente a N en
escala log-log para los dos casos, usando Excel.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <time.h>
#define LIMITE 4.0

typedef struct punto_t{
	double x;
	double y;
	double z;
}punto_t;

typedef struct caja_t{
	double limX [2];
	double limY [2];
	double limZ [2];
	double volumen;
}caja_t;

typedef enum limites_e{
	LIMITE_INF, LIMITE_SUP
}limites_e;

double generarNumeroAzar(double limInf, double limSup);
punto_t generarPunto(double* limX, double* limY, double* limZ);
caja_t generarCaja(double limInf_X, double limSup_X, double limInf_Y, double limSup_Y, double limInf_Z, double limSup_Z);
void calcularVolumenCaja(caja_t* caja);
double calcularAreaRectangulo(double largo, double ancho);
double calcularVolumenRevolucion(int num_puntos, caja_t caja, char eje);
int main(int argc, char** argv){
	
	//Inicializamos la semilla
	srand(time(0));
	
	//Para el csv
	#ifdef _WIN32
		setlocale(LC_NUMERIC, "Spanish");
	#else
		setlocale(LC_NUMERIC, "es_ES.UTF-8");
	#endif
	
	//Inicializamos las variables
	caja_t cajaX = generarCaja(-LIMITE, LIMITE, -1, 1, -1, 1);
	caja_t cajaY = generarCaja(-LIMITE, LIMITE, 0, 1, -LIMITE, LIMITE);
	double volumenEstimadoX = 0.0;
	double volumenEstimadoY = 0.0;
	
	//Calculamos los volumenes de las cajas
	calcularVolumenCaja(&cajaX);
	calcularVolumenCaja(&cajaY);
	
	//Calculamos varios volumenes y sus respectivos errores relativos
	for(int num_puntos = 100; num_puntos <= pow(10, 7); num_puntos *= 10){
		volumenEstimadoX = calcularVolumenRevolucion(num_puntos, cajaX, 'X');
		volumenEstimadoY = calcularVolumenRevolucion(num_puntos, cajaY, 'Y');
		printf("Numero de puntos: %d; Volumen estimado X: %lf; Volumen estimado Y: %lf\n", num_puntos, volumenEstimadoX, volumenEstimadoY);
	}
	
	return 0;
}

double generarNumeroAzar(double limInf, double limSup){
	double randLocal = 0.0f;
	randLocal = rand();
	
	return ((randLocal / RAND_MAX) * (limSup - limInf) + limInf);
}

punto_t generarPunto(double* limX, double* limY, double* limZ){
	
	//Generamos un punto con las especificaciones recibidas
	punto_t punto;
	punto.x = generarNumeroAzar(limX[LIMITE_INF], limX[LIMITE_SUP]);
	punto.y = generarNumeroAzar(limY[LIMITE_INF], limY[LIMITE_SUP]);
	punto.z = generarNumeroAzar(limZ[LIMITE_INF], limZ[LIMITE_SUP]);
	
	//Devolvemos el punto
	return punto;
}

caja_t generarCaja(double limInf_X, double limSup_X, double limInf_Y, double limSup_Y, double limInf_Z, double limSup_Z){
	
	//Generamos la caja alrededor del eje X
	caja_t caja;
	caja.limX[LIMITE_INF] = limInf_X;
	caja.limX[LIMITE_SUP] = limSup_X;
	caja.limY[LIMITE_INF] = limInf_Y;
	caja.limY[LIMITE_SUP] = limSup_Y;
	caja.limZ[LIMITE_INF] = limInf_Z;
	caja.limZ[LIMITE_SUP] = limSup_Z;
	caja.volumen = 0;
	
	//Devolvemos la caja
	return caja;
}

void calcularVolumenCaja(caja_t* caja){
	double areaBase = calcularAreaRectangulo(caja->limX[LIMITE_SUP] - caja->limX[LIMITE_INF], caja->limZ[LIMITE_SUP] - caja->limZ[LIMITE_INF]);
	double h = caja->limY[LIMITE_SUP] - caja->limY[LIMITE_INF];
	caja->volumen = areaBase * h;
}

double calcularAreaRectangulo(double largo, double ancho){
	return largo * ancho;
}

double calcularVolumenRevolucion(int num_puntos, caja_t caja, char eje){
	//definimos las variables
	int puntosDentro = 0;
	
	//Comprobación de errores
	if(!(eje == 'X' || eje == 'x' || eje == 'Y' || eje == 'y')){
		fprintf(stderr, "Error en la seleccion del eje");
		return 0.0;
	}
	
	for(int i = 0; i < num_puntos; i++){
		punto_t punto = generarPunto(caja.limX, caja.limY, caja.limZ);
		if(eje == 'X' || eje == 'x'){
			puntosDentro += (pow(punto.y, 2) + pow(punto.z, 2) <= exp(-2*pow(punto.x, 2)));
		}else{
			puntosDentro += (punto.y <= 1/exp(pow(punto.x, 2) + pow(punto.z, 2)));
		}
	}
	
	return caja.volumen * ((double)puntosDentro/num_puntos);
}