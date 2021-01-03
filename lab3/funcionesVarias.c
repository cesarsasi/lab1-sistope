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

float calculoPotenciaParcial(float ** matriz, cantidad){
    float potenciaParcial = 0;
    for (int i = 0; i < cantidad; ++i){
        potenciaParcial += pow(matriz[i][2],2) + sqrt(pow(matriz[i][3],2));
    }
    return potenciaParcial;
}

float calculoRuidoTotalParcial(float ** matriz, cantidad){
    float ruidoTotalParcial = 0;
    for (int i = 0; i < cantidad; ++i){
        potenciaParcial += matriz[i][4];
    }
    return potenciaParcial;
    
}

float calculoMediaReal(float matriz[3][5], int largo){
    float sumaReal=0.0;
    float largoF= (float)largo;
    float mediaR;
    for(int i=0;i<largo;i++){
        sumaReal=sumaReal+matriz[i][2];
    }
    mediaR = sumaReal/largoF;
    return mediaR;
}
float calculoMediaImaginaria(float matriz[3][5], int largo){
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
void calculador(){
	float ** matriz
	float resultadoParcial[4];
	resultadoParcial[0] = calculoPotenciaParcial(float ** matriz, cantidad);
	resultadoParcial[1] =  calculoRuidoTotalParcial(float ** matriz, cantidad);
	resultadoParcial[2] =  calculoMediaReal(float matriz[3][5], int largo);
	resultadoParcial[3] =  calculoMediaImaginaria(float matriz[3][5], int largo);

	//Escribir en la estructura correspondiente y guardar en el monitor con la estructura Manejo parcial

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
		}
	}
	fclose(archivoEntrada);
}

void crearMonitores(Monitor * listaMonitores){
    //crear monitor
    listaMonitores= (Monitor)calloc(sizeof(Monitor),cantDiscos);
    for(int i=0;i<cantDiscos;i++){
    	pthread_mutex_init(&listaMonitores[i]->mutex, NULL);
    	pthread_cond_init(&listaMonitores[i]->notFull, NULL);
    	pthread_cond_init(&listaMonitores[i]->notEmpty, NULL);
    	&listaMonitores[i]->indiceUltimo=0;
    	&listaMonitores[i]->tamañoBUffer=buffer;
    	&listaMonitores[i]->subMatriz=(float**)calloc(sizeof(float*),buffer);
    	for(int j=0;j<5;j++){
    		&listaMonitores[i]->subMatriz[j]=(float*)calloc(sizeof(float),5);
    	}
    }
}

/*
0 u
1 v
2 r
3 i
4 w */
