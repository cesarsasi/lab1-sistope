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

//FUNCIONES-------------------------------------------------------------------------------------------------------------------------
void escrituraResultados(char * archivoFinal, int idDisco, int mediaReal, int mediaImag, int potencia, int ruidoTotal){
    FILE* fileFinal = fopen(archivoFinal, "a");
    fprintf(fileFinal,"Disco: %d",idDisco);
    fprintf(fileFinal,"Media real: %d",mediaReal);
    fprintf(fileFinal,"Media imaginaria: %d",mediaImag);
    fprintf(fileFinal,"Potencia: %d",potencia);
    fprintf(fileFinal,"Ruido total %d:",ruidoTotal);
}

double calculoPotenciaParcial(double ** matriz, int largo){
    double potenciaParcial = 0;
    for (int i = 0; i < largo; ++i){
        potenciaParcial += pow(matriz[i][2],2) + sqrt(pow(matriz[i][3],2));
    }
    return potenciaParcial/largo;
}

double calculoRuidoTotalParcial(double ** matriz, int largo){
    double ruidoTotalParcial = 0;
    for (int i = 0; i < largo; ++i){
        ruidoTotalParcial += matriz[i][4];
    }
    return ruidoTotalParcial;
    
}

double calculoMediaReal(double ** matriz, int largo){
    double sumaReal=0.0;
    double largoF= (double)largo;
    double mediaR;
    for(int i=0;i<largo;i++){
        sumaReal=sumaReal+matriz[i][2];
    }
    mediaR = sumaReal/largoF;
    return mediaR;
}

double calculoMediaImaginaria(double ** matriz, int largo){
    double sumaImaginaria =0.0;
    double largoF=(double)largo;
    double mediaI;
    for(int i=0;i<largo;i++){
        sumaImaginaria=sumaImaginaria+matriz[i][3];
    }
    mediaI= sumaImaginaria/largoF;
    return mediaI;
}
//*******************************************************************************************************************************
//Calculador reune las funciones.h y las ejecuta 
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
		resultadoParcial[2] = calculoPotenciaParcial(monitor->subMatriz, monitor->indiceUltimo);
		resultadoParcial[3] = calculoRuidoTotalParcial(monitor->subMatriz, monitor->indiceUltimo);
		resultadoParcial[0] = calculoMediaReal(monitor->subMatriz, monitor->indiceUltimo);         //-------------------------------------Modificar a calculo real!!!!!
		resultadoParcial[1] = calculoMediaImaginaria(monitor->subMatriz, monitor->indiceUltimo);   //-------------------------------------Modificar a calculo real!!!!!
		/*if (true){
			printf("\n DISCO %d",monitor->idMonitor);
			printf("\n RP1 %lf ",resultadoParcial[0]);
			printf("\n RP2 %lf ",resultadoParcial[1]);
			printf("\n RP3 %lf ",resultadoParcial[2]);
			printf("\n RP4 %lf ",resultadoParcial[3]);
		}*/
		for (int i = 0; i < 4; i++){
			comun.resultadoTotalDiscos[monitor->idMonitor-1][i] += resultadoParcial[i];
		}
		//Vaciar submatriz y reestablecer los datos del monitor
		/*for (int i = 0; i < buffer; i++){
			//free(monitor->subMatriz[i]);
		}*/
		//free(monitor->subMatriz);
		monitor->indiceUltimo = 0;
		monitor->full=2;
		//Liberar la hebra
		pthread_cond_signal(&monitor->notFull);
		pthread_mutex_unlock(&monitor->mutex);
	}
	pthread_exit(NULL);

}

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

		//printf("\n %lf,%lf ",posU,posV);
		double dist = sqrt(sumPot);
		//printf("\n %lf ------Dist",dist);
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

		//printf("\n %d ------------------------------    INDICE",indiceDiscAsignado);
		//con este for identificamos el monitor que debemos usar
		
		if(listaMonitores[indiceDiscAsignado].indiceUltimo < buffer){
			int indice=listaMonitores[indiceDiscAsignado].indiceUltimo;
			listaMonitores[indiceDiscAsignado].subMatriz[indice][0]= posU;
			listaMonitores[indiceDiscAsignado].subMatriz[indice][1]= posV;
			listaMonitores[indiceDiscAsignado].subMatriz[indice][2]= posR;
			listaMonitores[indiceDiscAsignado].subMatriz[indice][3]= posI;
			listaMonitores[indiceDiscAsignado].subMatriz[indice][4]= posRU;
			listaMonitores[indiceDiscAsignado].indiceUltimo = indice+1;
			//printf("\n %d ------------------------------    INDICE",listaMonitores[indiceDiscAsignado].indiceUltimo);
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
		//printf("\n %d ------------------------------    Final",listaMonitores[indiceDiscAsignado].indiceUltimo);
		//printf("\n %d ------------------------------    Final",i);
	}
	terminoLectura = 1;
	//printf("333333333333333333333333333333333333333333333333333");
	for(int i=0;i<cantDiscos;i++){
		listaMonitores[i].full=1;
		while (listaMonitores[i].full==1){
			pthread_cond_signal(&listaMonitores[i].notEmpty);
			pthread_cond_wait(&listaMonitores[i].notFull, &listaMonitores[i].mutex);
		}
		pthread_mutex_unlock(&listaMonitores[i].mutex);
	}
	//Leer lineas que quedan washitas
	fclose(archivoEntrada);
	
}

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
    	listaMonitores[i].subMatriz=(double**)calloc(sizeof(double*),buffer);
    	for(int j=0;j<buffer;j++){
    		listaMonitores[i].subMatriz[j]=(double*)calloc(sizeof(double),5);
    	}
    }
	for(int i=0;i<cantDiscos;i++){
		for(int j=0;j<buffer;j++){
			for(int k=0;k<5;k++){
				listaMonitores[i].subMatriz[j][k] = 0.0;
			}
		}
	}
	//Bloquear monitor previa creacion de hebra
	for (int i = 0; i < cantDiscos; i++){
		//pthread_mutex_lock(&listaMonitores[i].mutex);
	}
	
}


void iniciarEstructuraComun(){
	comun.resultadoTotalDiscos = (double**)calloc(sizeof(double*),cantDiscos);
    for (int i = 0; i < cantDiscos; i++){
        comun.resultadoTotalDiscos[i] = (double*)calloc(sizeof(double),4);
		for (int j = 0; j < 4; j++){
			comun.resultadoTotalDiscos[i][j]= 0;
		}
    }
    
}


//Imprimir matriz Prueba
	/*for(int i=0;i<largo;i++){
        for(int j=0;j<5;j++){
            printf("%lf",archivoGuardado[i][j]);
            printf(" ");
        }
        printf("\n");
    }*/ 
//FUNCIONES-------------------------------------------------------------------------------------------------------------------------