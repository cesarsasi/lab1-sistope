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

float calculoPotenciaParcial(float ** matriz, int largo){
    float potenciaParcial = 0;
    for (int i = 0; i < largo; ++i){
        potenciaParcial += pow(matriz[i][2],2) + sqrt(pow(matriz[i][3],2));
    }
    return potenciaParcial;
}

float calculoRuidoTotalParcial(float ** matriz, int largo){
    float ruidoTotalParcial = 0;
    for (int i = 0; i < largo; ++i){
        ruidoTotalParcial += matriz[i][4];
    }
    return ruidoTotalParcial;
    
}

float calculoMediaReal(float ** matriz, int largo){
    float sumaReal=0.0;
    float largoF= (float)largo;
    float mediaR;
    for(int i=0;i<largo;i++){
        sumaReal=sumaReal+matriz[i][2];
    }
    mediaR = sumaReal/largoF;
    return mediaR;
}

float calculoMediaImaginaria(float ** matriz, int largo){
    float sumaImaginaria =0.0;
    float largoF=(float)largo;
    float mediaI;
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
	while(true){
		//si ya esta tomado el mutex la hebra se bloquea
		pthread_mutex_lock (&monitor->mutex);
		//si el monitor no llena buffer no se ejecuta lo que continua despues del while
		while (monitor->empty) {
			pthread_cond_wait (&monitor->notEmpty, &monitor->mutex);
		}
		// el buffer esta lleno, por lo tanto se ejecuta la logica del lab
		float resultadoParcial[4];
		resultadoParcial[0] = calculoPotenciaParcial(monitor->subMatriz, monitor->indiceUltimo);
		resultadoParcial[1] = calculoRuidoTotalParcial(monitor->subMatriz, monitor->indiceUltimo);
		resultadoParcial[2] = calculoMediaReal(monitor->subMatriz, monitor->indiceUltimo); //------------------------------------------Modificar a calculo real!!!!!
		resultadoParcial[3] = calculoMediaImaginaria(monitor->subMatriz, monitor->indiceUltimo);//-------------------------------------Modificar a calculo real!!!!!
		//Escribir en la estructura correspondiente y guardar en el monitor con la estructura Manejo parcial
		for (int i = 0; i < 5; i++){
			comun.resultadoTotalDiscos[monitor->idMonitor][i] += resultadoParcial[i];
		}
		//Vaciar submatriz y reestablecer los datos del monitor
		for (int i = 0; i < buffer; i++){
			//free(monitor->subMatriz[i]);
		}
		//free(monitor->subMatriz);
		monitor->indiceUltimo = 0;

		//Liberar la hebra
		pthread_cond_signal(&monitor->notFull);
		pthread_mutex_unlock(&monitor->mutex);
	}
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
	float **archivoGuardado = (float **)calloc(sizeof(float*), largo);
	for(int i=0; i<largo;i++){
		archivoGuardado[i]=(float*)calloc(sizeof(float),5);
	}
	float posU, posV,posR,posI,posRU;
	archivoEntrada=fopen(archivoVisibilidades,"r");
	while(true){

		for(int i=0; i<largo;i++){
			for(int j=0;j<5;j++){
				fscanf(archivoEntrada, "%f", &posU);
				c=fgetc(archivoEntrada);

				fscanf(archivoEntrada, "%f", &posV);
				c=fgetc(archivoEntrada);
				
				fscanf(archivoEntrada, "%f", &posR);
				c=fgetc(archivoEntrada);

				fscanf(archivoEntrada, "%f", &posI);
				c=fgetc(archivoEntrada);

				fscanf(archivoEntrada, "%f", &posRU);
				c=fgetc(archivoEntrada);
				
				//Productor calcula propiedades del consumidor y consumidor es el que procesa
				//Distribucion y como identificar cuando un punto va a un monitor
				
				//Obtenemos su distancia del centro}
				float sumPot = pow(posU,2) + pow(posV,2);
				float dist = sqrt(sumPot);
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
					listaMonitores[indiceDiscAsignado].subMatriz[listaMonitores->indiceUltimo][0]=posU;
					listaMonitores[indiceDiscAsignado].subMatriz[listaMonitores->indiceUltimo][1]=posV;
					listaMonitores[indiceDiscAsignado].subMatriz[listaMonitores->indiceUltimo][2]=posR;
					listaMonitores[indiceDiscAsignado].subMatriz[listaMonitores->indiceUltimo][3]=posI;
					listaMonitores[indiceDiscAsignado].subMatriz[listaMonitores->indiceUltimo][4]=posRU;
					listaMonitores->indiceUltimo+=1;
				}
				// se termina la produccion
				if(listaMonitores[indiceDiscAsignado].indiceUltimo == buffer){//el buffer queda lleno
					//bloqueamos el mutex del monitor
					pthread_mutex_lock (&listaMonitores[indiceDiscAsignado].mutex);
					//mientras este lleno se bloquea
					while (listaMonitores[indiceDiscAsignado].full) {
						pthread_cond_wait (&listaMonitores[indiceDiscAsignado].notFull, &listaMonitores[indiceDiscAsignado].mutex);
					}
					//se libera el monitor correspondiente que ya previamente vacia el buffer del monitor y permite proceder la lectura
					pthread_cond_signal(&listaMonitores[indiceDiscAsignado].notEmpty);
					pthread_mutex_unlock(&listaMonitores[indiceDiscAsignado].mutex);
				}	
				
				
			}
		}
	//Leer lineas que quedan washitas
	}
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
    	listaMonitores[i].subMatriz=(float**)calloc(sizeof(float*),buffer);
    	for(int j=0;j<5;j++){
    		listaMonitores[i].subMatriz[j]=(float*)calloc(sizeof(float),5);
    	}
    }
}


void iniciarEstructuraComun(){
	comun.resultadoTotalDiscos = (float**)calloc(sizeof(float*),cantDiscos);
    for (int i = 0; i < cantDiscos; i++){
        comun.resultadoTotalDiscos[i] = (float*)calloc(sizeof(float),5);
		for (int j = 0; j < 5; j++){
			comun.resultadoTotalDiscos[i][j]= 0;
		}
    }
    
}


//Imprimir matriz Prueba
	/*for(int i=0;i<largo;i++){
        for(int j=0;j<5;j++){
            printf("%f",archivoGuardado[i][j]);
            printf(" ");
        }
        printf("\n");
    }*/ 
//FUNCIONES-------------------------------------------------------------------------------------------------------------------------