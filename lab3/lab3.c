#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

#include "Declaracion.h"

// ./lab3 -i prueba1.csv -o propiedades.txt -d 100 -n 4 -s 10 -b
int main(int argc, char** argv){
	//Lectura parametros desde la terminal
	while (( (d = getopt(argc, argv, "i:o:n:s:b")) != -1)){
        switch (d)
        {
        case 'i':
        mflag = optarg; 
        if(optarg == 0){
            printf("\nIngreso de parametro i incorrecto! \n");
            exit(EXIT_FAILURE);
        }else{
            archivoVisibilidades = optarg;
            FILE* fileVisib = fopen(archivoVisibilidades, "r");
            if(fileVisib==NULL){
            printf("\nIngreso de parametro i (nombre archivo de vidibilidades) incorrecto!, el archivo no existe \n");
            exit(EXIT_FAILURE);
            }
            fclose(fileVisib);
        }
        break;
        case 'o':
        mflag = optarg; 
        if(optarg == 0){
            printf("\nIngreso de parametro o incorrecto! \n");
            exit(EXIT_FAILURE);
        }else{
            archivoSalida = optarg;
            FILE* fileSalida = fopen(archivoSalida, "w");
            if(fileSalida==NULL){
            printf("\nIngreso de parametro o (nombre archivo de salida) incorrecto!, el archivo no existe \n");
            exit(EXIT_FAILURE);
            }
            fclose(fileSalida);
        }
        break;
        case 'n':
        mflag = optarg; 
        if(optarg == 0){
            printf("\nIngreso de parametro n (cantidad de discos) incorrecto!\n");
            exit(EXIT_FAILURE);
        }else{
            cantDiscos = atoi(optarg);
            if(cantDiscos >= 0){
            break;
            }
            else{
            printf("\nIngreso de parametro n (cantidad de discos) incorrecto!, debe ser mayor que cero\n"); //Verificacion pendiente
            exit(EXIT_FAILURE);
            }
        }
        case 'd':
        mflag = optarg; 
        if(optarg == 0){
            printf("\nIngreso de parametro d (ancho de discos) incorrecto!\n");
            exit(EXIT_FAILURE);
        }else{
            anchoDiscos = atoi(optarg);
            if(anchoDiscos >= 0){
            break;
            }
            else{
            printf("\nIngreso de parametro d (ancho de discos) incorrecto!, debe ser mayor que cero\n"); //Verificacion pendiente
            exit(EXIT_FAILURE);
            }
        }
        break;      
        case 's':
        mflag = optarg; 
        if(optarg == 0){
            printf("\nIngreso de parametro s (buffer) incorrecto!\n");
            exit(EXIT_FAILURE);
        }else{
            buffer = atoi(optarg);
            if(buffer >= 0){
            break;
            }
            else{
            printf("\nIngreso de parametro s (buffer) incorrecto!, debe ser igual o mayor que cero\n"); //Verificacion pendiente
            exit(EXIT_FAILURE);
            }
        }
        break;
        case 'b':
        flag = 1;
        break;
        }
    }

	//Creamos y Seteamos monitores
	crearMonitor(listaMonitores);

	//crear y ejecutar hebras de discos
    pthread_t* hebras;
    hebras = (pthread_t)calloc(sizeof(pthread_t),cantDiscos);
    for(int i= 0; i<cantDiscos;i++){
        pthread_create(&hebras[i], NULL, calculador, (void *) listaMonitores[i]);
    }

	//Lectura de lineas y asignacion de variables al monitor
	asignarDataMonitores(archivoVisibilidades,listaMonitores);

	//Imprimir matriz Prueba
	/*for(int i=0;i<largo;i++){
        for(int j=0;j<5;j++){
            printf("%f",archivoGuardado[i][j]);
            printf(" ");
        }
        printf("\n");
    }*/

	

    
    
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

    

    //Juntar resultados en las hebras y mandarlo a la estructura de resultado total

    //Printear o escribir en documento

	return 0;
}