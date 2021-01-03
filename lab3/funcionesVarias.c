#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#include "Declaracion.h"

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
////////////////////******************************************************************************************************
//Calculador reune las funciones.h y las ejecuta 
void calculador(void * monitorVoid){
	Monitor * monitor = (Monitor *)monitorVoid;
	
	float resultadoParcial[4];
	resultadoParcial[0] = calculoPotenciaParcial(monitor.subMatriz, monitor.indiceUltimo);
	resultadoParcial[1] = calculoRuidoTotalParcial(monitor.subMatriz, monitor.indiceUltimo);
	resultadoParcial[2] = calculoMediaReal(monitor.subMatriz, monitor.indiceUltimo); //------------------------------------------Modificar a calculo real!!!!!
	resultadoParcial[3] = calculoMediaImaginaria(monitor.subMatriz, monitor.indiceUltimo);//-------------------------------------Modificar a calculo real!!!!!
	//Escribir en la estructura correspondiente y guardar en el monitor con la estructura Manejo parcial
	comun[monitor.idMonitor] = [resultadoParcial[0],resultadoParcial[1],resultadoParcial[2],resultadoParcial[3]]
	//devolverse al productor
}

void asignarDataMonitores(char * archivoVisibilidades, Monitor * listaMonitores){
    int largo = 0;
	archivoEntrada=fopen(archivoVisibilidades,"r");
	char c;
	int datos;
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
	archivoEntrada=fopen(archivoVisibilidades,"r");
	for(int i=0; i<largo;i++){
		for(int j=0;j<5;j++){
			fscanf(archivoEntrada, "%f", &archivoGuardado[i][j]);
			c=fgetc(archivoEntrada);
			/*
			//Productor calcula propiedades del consumidor y consumidor es el que calcula todu
			//Distribucion y como identificar cuando un punto va a un monitor
			for(int i = 0; i < largo; i ++){
				//Leemos la linea en particular
				float posU = archivoGuardado[i][0];
				float posV = archivoGuardado[i][1];
				//Obtenemos su distancia del centro}
				float sumPot = pow(posU,2) + pow(posV,2);
				float dist = sqrt(sumPot);
				//calculamos en que disco queda asignada (indice del monitor en el arreglo)
				int indiceDiscAsignado = -1;
				for (int j = 0; j < cantDiscos; ++j){
					if( rangosDisco[j]<dist && rangosDisco[j]+anchoDiscos>dist){
						indiceDiscAsignado = j;
					}
				}
				if(indiceDiscAsignado != -1){
					//con este for identificamos el monitor que debemos usar
					if(&listaMonitores->indiceUltimo < buffer){
						&listaMonitores[indiceDiscAsignado]->subMatriz[&listaMonitores->indiceUltimo][0]=archivoGuardado[i][0];
						&listaMonitores[indiceDiscAsignado]->subMatriz[&listaMonitores->indiceUltimo][1]=archivoGuardado[i][1];
						&listaMonitores[indiceDiscAsignado]->subMatriz[&listaMonitores->indiceUltimo][2]=archivoGuardado[i][2];
						&listaMonitores[indiceDiscAsignado]->subMatriz[&listaMonitores->indiceUltimo][3]=archivoGuardado[i][3];
						&listaMonitores[indiceDiscAsignado]->subMatriz[&listaMonitores->indiceUltimo][4]=archivoGuardado[i][4];
						&listaMonitores->indiceUltimo+=1;
					}
					//el buffer queda lleno
					else{
						//ejecutamos el proceso y hacemos que la hebra se ejecute haciendo esperar la lectura
						pthread_create(&hebra[indiceDiscAsignado], NULL, calculador, (void *) &listaMonitores[indiceDiscAsignado]);
						//se vacia el buffer del monitor y continua la lectura
					}	
				}
			}
			*/
		}
	}
	fclose(archivoEntrada);
}

void crearMonitores(Monitor * listaMonitores){
    //crear monitor
    listaMonitores = (Monitor*)calloc(sizeof(Monitor),cantDiscos);
    for(int i=0;i<cantDiscos;i++){
    	pthread_mutex_init(&listaMonitores[i]->mutex, NULL);
    	pthread_cond_init(&listaMonitores[i]->notFull, NULL);
    	pthread_cond_init(&listaMonitores[i]->notEmpty, NULL);
    	listaMonitores[i].indiceUltimo=0;
    	listaMonitores[i].tamañoBUffer=buffer;
		listaMonitores[i].idMonitor=i+1;
    	listaMonitores[i].subMatriz=(float**)calloc(sizeof(float*),buffer);
    	for(int j=0;j<5;j++){
    		listaMonitores[i].subMatriz[j]=(float*)calloc(sizeof(float),5);
    	}
    }
}

/*
0 u
1 v
2 r
3 i
4 w */
