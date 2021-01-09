/*
Laboratorio 3
Profesor   : Fernando Rannou
Ayudante   : Benjamin Muñoz
Integrantes:
-                Octavio Nicolas Canales Ñirriman 20.003.610-7
-                César Antonio Salazar Silva      19.916.471-6
Sistemas Operativos 2.2020  (Creación: 26 de Diciembre del 2020)
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>

#include "Declaracion.h"
/*
0 u
1 v
2 r
3 i
4 w */

/* FUNCION : escribe en el archivo de salida los resultados obtenidos con la ejecucion del programa
ENTRADA: recibe el nombre del archivo final y lista de monitores
SALIDA : void
*/
void escrituraResultados(char * archivoFinal, int idDisco, int mediaReal, int mediaImag, int potencia, int ruidoTotal){
    FILE* fileFinal = fopen(archivoFinal, "a");
    fprintf(fileFinal,"Disco: %d",idDisco);
    fprintf(fileFinal,"Media real: %d",mediaReal);
    fprintf(fileFinal,"Media imaginaria: %d",mediaImag);
    fprintf(fileFinal,"Potencia: %d",potencia);
    fprintf(fileFinal,"Ruido total %d:",ruidoTotal);
}
/* FUNCION : calcula la pontencia parcial de un disco
ENTRADA: recibe la matriz que contiene los datos del disco  y el largo
SALIDA : retorna la potencia parcial en formato double
*/
double calculoPotenciaParcial(double ** matriz, int largo){
    double potenciaParcial = 0;
    for (int i = 0; i < largo; ++i){
        potenciaParcial += sqrt(pow(matriz[i][2],2) + pow(matriz[i][3],2));
    }
    return potenciaParcial;
}
/* FUNCION : calcula el ruido total parcial de cada disco
ENTRADA: recibe la matriz que contiene los datos del disco y largo
SALIDA : retorna ruido total parcial en formato double
*/
double calculoRuidoTotalParcial(double ** matriz, int largo){
    double ruidoTotalParcial = 0;
    for (int i = 0; i < largo; ++i){
        ruidoTotalParcial += matriz[i][4];
    }
    return ruidoTotalParcial;
}
/* FUNCION : calcula la suma correspondiente a los vectores reales, no los divide debido a que esto se hace al final
ENTRADA: recibe la matriz que contiene los datos del disco y largo
SALIDA : retorna la suma de la media real en formato double
*/
double calculoSumaMediaReal(double ** matriz, int largo){
    double sumaReal=0.0;
    for(int i=0;i<largo;i++){
        sumaReal=sumaReal+matriz[i][2];
    }
    return sumaReal;
}
/* FUNCION : calcula la suma correspondiendo a los vectoes imaginarios, no los divide debido a que esto se hace al final
ENTRADA: recibe la matriz que contiene los datos del disco y largo
SALIDA : retorna la suma de la media real en formato double
*/
double calculoSumaMediaImaginaria(double ** matriz, int largo){
    double sumaImaginaria =0.0;
    for(int i=0;i<largo;i++){
        sumaImaginaria=sumaImaginaria+matriz[i][3];
    }
    return sumaImaginaria;
}
/* FUNCION : funcion que calcula en los discos los resultados parciales, esta es ejecutada por las hebras. (Consumidor)
ENTRADA: recibe el monitor correspondiente en formato void*
SALIDA : void
OBS    :  a pesar de no retornar parametros, la funcion modifica variables globales 
*/
void * calculador(void * monitorVoid){
	Monitor * monitor = (Monitor *)monitorVoid;
	//si ya esta tomado el mutex la hebra se bloquea
	while(terminoLectura == 0){
		pthread_mutex_lock (&monitor->mutex);
		//si el monitor no llena buffer no se ejecuta lo que continua despues del while
		if(monitor->indiceUltimo != buffer || monitor->full==1) {
			pthread_cond_wait (&monitor->notEmpty, &monitor->mutex);
		}
		// el buffer esta lleno, por lo tanto se ejecuta la logica del lab
		double resultadoParcial[4];
		resultadoParcial[0] = 0;
		resultadoParcial[1] = 0;
		resultadoParcial[2] = 0;
		resultadoParcial[3] = 0;
		resultadoParcial[0] = calculoSumaMediaReal(monitor->subMatriz, monitor->indiceUltimo);         //-------------------------------------Modificar a calculo real!!!!!
		resultadoParcial[1] = calculoSumaMediaImaginaria(monitor->subMatriz, monitor->indiceUltimo);   //-------------------------------------Modificar a calculo real!!!!!
		resultadoParcial[2] = calculoPotenciaParcial(monitor->subMatriz, monitor->indiceUltimo);
		resultadoParcial[3] = calculoRuidoTotalParcial(monitor->subMatriz, monitor->indiceUltimo);
		for (int i = 0; i < 4; i++){
			monitor->resultadoTotalDiscos[monitor->idMonitor-1][i] += resultadoParcial[i];
		}
		//Vaciar submatriz y reestablecer los datos del monitor
		monitor->indiceUltimo = 0;
		monitor->full=2;
		//Liberar la hebra
		pthread_cond_signal(&monitor->notFull);
		pthread_mutex_unlock(&monitor->mutex);
	}
	pthread_exit(NULL);
}
/* FUNCION : Genera la lectura y en base a las visibilidades leidas introduce en el monitor de Disco correspondiente segun calculo previo
ENTRADA: Ninguna, gestiona variables globales.
SALIDA : Ninguna, gestiona y maneja variables globales.
OBS    : Funcion a lo que en catedra llamamos Productor, que llama a los consumidores de cada monitor dependiendo sea el caso.
*/
void asignarDataMonitores(){
    int largo = 0;
	archivoEntrada=fopen(archivoVisibilidades,"r");
	char c;
	while((c = fgetc(archivoEntrada)) != EOF){
		if(c == '\n'){
			largo++;
		}
	}
	fclose(archivoEntrada);
	printf("\n largo archivo %d \n", largo);
	double **archivoGuardado = (double **)calloc(sizeof(double*), largo);
	for(int i=0; i<largo;i++){
		archivoGuardado[i]=(double*)calloc(sizeof(double),5);
	}
	double posU, posV,posR,posI,posRU;
	archivoEntrada=fopen(archivoVisibilidades,"r");
	for(int i=0; i<largo;i++){
		fscanf(archivoEntrada, "%lf", &posU);
		c=fgetc(archivoEntrada);
		fscanf(archivoEntrada, "%lf", &posV);
		c=fgetc(archivoEntrada);
		
		fscanf(archivoEntrada, "%lf", &posR);
		c=fgetc(archivoEntrada);

		fscanf(archivoEntrada, "%lf", &posI);
		c=fgetc(archivoEntrada);

		fscanf(archivoEntrada, "%lf", &posRU);
		c=fgetc(archivoEntrada);
		//Productor calcula propiedades del consumidor y consumidor es el que procesa
		//Distribucion y como identificar cuando un punto va a un monitor
		//Obtenemos su distancia del centro}
		double sumPot = pow(posU,2) + pow(posV,2);
		double dist = sqrt(sumPot);
		//calculamos en que disco queda asignada (indice del monitor en el arreglo)
		int indiceDiscAsignado = -1;
		for (int j = 0; j < cantDiscos; ++j){
			if( (anchoDiscos*j)<=dist && (anchoDiscos*j)+anchoDiscos>dist){
				indiceDiscAsignado = j;
			}
		}
		if (indiceDiscAsignado == -1){
			indiceDiscAsignado = cantDiscos -1;
		}
		//con este for identificamos el monitor que debemos usar
		if(listaMonitores[indiceDiscAsignado].indiceUltimo < buffer){
			int indice=listaMonitores[indiceDiscAsignado].indiceUltimo;
			listaMonitores[indiceDiscAsignado].subMatriz[indice][0]= posU;
			listaMonitores[indiceDiscAsignado].subMatriz[indice][1]= posV;
			listaMonitores[indiceDiscAsignado].subMatriz[indice][2]= posR;
			listaMonitores[indiceDiscAsignado].subMatriz[indice][3]= posI;
			listaMonitores[indiceDiscAsignado].subMatriz[indice][4]= posRU;
			listaMonitores[indiceDiscAsignado].indiceUltimo = indice+1;
			listaMonitores[indiceDiscAsignado].lineasLeidas+=1;
		}
		// se termina la produccion
		if(listaMonitores[indiceDiscAsignado].indiceUltimo == buffer){//el buffer queda lleno
			//bloqueamos el mutex del monitor
			pthread_mutex_lock(&listaMonitores[indiceDiscAsignado].mutex);
			//mientras este lleno se bloquea
			if(listaMonitores[indiceDiscAsignado].indiceUltimo==buffer){
				pthread_cond_signal(&listaMonitores[indiceDiscAsignado].notEmpty);
				pthread_cond_wait(&listaMonitores[indiceDiscAsignado].notFull, &listaMonitores[indiceDiscAsignado].mutex);
			}
			//se libera el monitor correspondiente que ya previamente vacia el buffer del monitor y permite proceder la lectura
			//pthread_cond_signal(&listaMonitores[indiceDiscAsignado].notEmpty);
			pthread_mutex_unlock(&listaMonitores[indiceDiscAsignado].mutex);
		}
	}
	terminoLectura = 1;
	for(int i=0;i<cantDiscos;i++){
		listaMonitores[i].full=1;
		while (listaMonitores[i].full==1){
			pthread_cond_signal(&listaMonitores[i].notEmpty);
			pthread_cond_wait(&listaMonitores[i].notFull, &listaMonitores[i].mutex);
		}
		pthread_mutex_unlock(&listaMonitores[i].mutex);
	}
	//termina hebra se escribe en comun
	for(int i=0;i<cantDiscos;i++){
		Ecomun.resultadoTotalDiscos[i][0]= listaMonitores[i].resultadoTotalDiscos[i][0]/listaMonitores[i].lineasLeidas;//media real
		Ecomun.resultadoTotalDiscos[i][1]= listaMonitores[i].resultadoTotalDiscos[i][1]/listaMonitores[i].lineasLeidas;//media imaginaria
		Ecomun.resultadoTotalDiscos[i][2]= listaMonitores[i].resultadoTotalDiscos[i][2];//potencia par
		Ecomun.resultadoTotalDiscos[i][3]= listaMonitores[i].resultadoTotalDiscos[i][3];//ruido
	}
	fclose(archivoEntrada);
}
/* FUNCION : Inicializa la estructura monitor, asigna memoria a las variables correspondientes de cada uno y los settea en cero.
ENTRADA: Ninguna, maneja puntero tipo Monitor que es variable global
SALIDA : Ninguna, maneja datos y variables de puntero Monitor.
OBS    : Es una lista de monitores la que crea
*/
void crearMonitores(){
    //crear monitor
    listaMonitores = (Monitor*)calloc(sizeof(Monitor),cantDiscos);
    for(int i=0;i<cantDiscos;i++){
    	pthread_mutex_init(&listaMonitores[i].mutex, NULL);
    	pthread_cond_init(&listaMonitores[i].notFull, NULL);
    	pthread_cond_init(&listaMonitores[i].notEmpty, NULL);
    	listaMonitores[i].indiceUltimo=0;
    	listaMonitores[i].tamanoBUffer=buffer;
		listaMonitores[i].idMonitor=i+1;
		listaMonitores[i].full=0;
		listaMonitores[i].lineasLeidas=0;
    	listaMonitores[i].subMatriz=(double**)calloc(sizeof(double*),buffer);
    	for(int j=0;j<buffer;j++){
    		listaMonitores[i].subMatriz[j]=(double*)calloc(sizeof(double),5);
    	}
		listaMonitores[i].resultadoTotalDiscos = (double**)calloc(sizeof(double*),cantDiscos);
		for (int j = 0; j < cantDiscos; j++){
			listaMonitores[i].resultadoTotalDiscos[j] = (double*)calloc(sizeof(double),4);
		}
    }
	for(int i=0;i<cantDiscos;i++){
		for(int j=0;j<buffer;j++){
			for(int k=0;k<5;k++){
				listaMonitores[i].subMatriz[j][k] = 0.0;
			}
		}
	}	
}
/* FUNCION : Inicia y asigna memoria correspondiente para la estructura en la que se almacenaran los resultados finales 
ENTRADA: Los paramentros que maneja son variables globales
SALIDA : Ninguna, modifica variables globales
OBS    :
*/
void iniciarEstructuraComun(){
	Ecomun.resultadoTotalDiscos = (double**)calloc(sizeof(double*),cantDiscos);
    for (int i = 0; i < cantDiscos; i++){
        Ecomun.resultadoTotalDiscos[i] = (double*)calloc(sizeof(double),4);
		for (int j = 0; j < 4; j++){
			Ecomun.resultadoTotalDiscos[i][j]= 0;
		}
    }
    
}