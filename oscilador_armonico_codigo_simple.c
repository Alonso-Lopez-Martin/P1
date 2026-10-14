/* Introducción a la programación científica                                     */
/* Grado en Física Computacional. Centro Universitario U-tad                     */
/* Autor: Javier García Algarra                                                  */
/* Agosto de 2023                                                                */

/* Programa para integrar la ecuación  m (d2x/dt2) =  -k x
 * con condiciones iniciales x(0) = 5; v(0) = 0
 * 
 * Hav que convertir la ecuación de segundo orden en un
 * sistema de dos ecuaciones acopladas
 * 
 * dx/dt = v
 * dv/dt = -k/m x
 * 
 * */
 
#include <stdio.h>
#include<math.h>
#include<locale.h>

#define NUM_INTERVALOS 100
  
int main(){

  float t[NUM_INTERVALOS+1];
  float x[NUM_INTERVALOS+1];
  float v[NUM_INTERVALOS+1];
  
  float dx_dt[NUM_INTERVALOS+1], dv_dt[NUM_INTERVALOS+1];
  float k = 1.0, m = 1.0;
  float Deltat;
  float lim_inf = 0.0;
  float lim_sup = 20.0;
  char linea[255];


  #ifdef _WIN32
	setlocale(LC_NUMERIC, "Spanish");
  #else
	setlocale(LC_NUMERIC, "es_ES.UTF-8");
  #endif  

  Deltat = (lim_sup-lim_inf)/NUM_INTERVALOS;
  v[0] = 0;   // Condición inicial
  x[0] = 5;
  
  t[0] = lim_inf;
  printf("t\tx\tv\n");
  
  // Calculamos los instantes de la serie
    
  for (int i=1;i<NUM_INTERVALOS;i++){
	t[i] = i*Deltat+t[0];
		
	dv_dt[i-1] = -(k/m)*x[i-1];
	v[i] = v[i-1] + Deltat*dv_dt[i-1];
	dx_dt[i-1] = v[i-1];         // Euler original
	//dx_dt[i-1] = v[i];           // Euler-Cromer
	x[i] = x[i-1] + Deltat*dx_dt[i-1];
	
	printf("%0.2f\t%0.2f\t%0.2f\n",t[i],x[i],v[i]);
}

  
// Volcar la salida a un fichero .txt, y representar las gráficas con Excel 

}
