#ifndef funcionesComplejas_h_
#define funcionesComplejas_h_
#include "funcionesVarias.h"

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

#endif