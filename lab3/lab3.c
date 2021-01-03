#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#include "funcionesComplejas.h"
#include "funcionesVarias.h"

//Variables globales
FILE *archivoEntrada;
char *mflag;
char* archivoVisibilidades,*archivoSalida;
int d;
int cantDiscos;
int buffer;
int anchoDiscos;
int flag;
Monitor *listaMonitores;


typedef struct monitor{
	float **subMatriz;
	int indiceUltimo;
	int tamañoBUffer;
	int head, tail;
	int full, empty;
	pthread_mutex_t mutex;
	pthread_cond_t notFull, notEmpty;
} Monitor;

// ./lab3 -i prueba1.csv -o propiedades.txt -d 100 -n 4 -s 10 -b
int main(int argc, char** argv){

	//Lectura parametros desde la terminal
	lecturaParametros(argc,argv);

	//Creamos y Seteamos monitores
	crearMonitor(listaMonitores);

	//crear y ejecutar hebras de discos
    pthread_t* hebras;
    hebras = (pthread_t)calloc(sizeof(pthread_t),cantDiscos);

	//Lectura de lineas y asignacion de variables al monitor
	asignarDataMonitores(archivoVisibilidades,listaMonitores);

	//Imprimir matriz Prueba
	for(int i=0;i<largo;i++){
        for(int j=0;j<5;j++){
            printf("%f",archivoGuardado[i][j]);
            printf(" ");
        }
        printf("\n");
    }

	

    
    
    /*
    for(int i=0; i < cantDiscos;i++){
        pthread_create(&hebras[i],NULL,imprimir,&i);
        pthread_join(hebras[i],NULL);//Cuando termine de leer y termine de procesar
    }*/

    //Calcular rango de cada disco
    int * rangosDisco = (int)calloc(sizeof(int),cantDiscos+1);
    for (int i = 0; i < cantDiscos; ++i){
    	rangosDisco[i] = anchoDiscos*i;
    }

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

    //Juntar resultados en las hebras y mandarlo a la estructura de resultado total

    //Printear o escribir en documento

	return 0;
}