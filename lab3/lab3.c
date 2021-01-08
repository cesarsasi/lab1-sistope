#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<pthread.h>
#include<stdbool.h>

#include "Declaracion.h"

// ./lab3 -i prueba1.csv -o propiedades.txt -d 100 -n 4 -s 10 -b

int main(int argc, char** argv){
	//Lectura parametros desde la terminal
    flag=0;
    terminoLectura=0;
	while (( (d = getopt(argc, argv, "i:o:d:n:s:b")) != -1)){
        switch (d){
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
        case 'd':
            mflag = optarg; 
            if(optarg == 0){
                printf("\nIngreso de parametro d (ancho de discos) incorrecto!\n");
                exit(EXIT_FAILURE);
            }else{
                anchoDiscos = atoi(optarg);
                if(anchoDiscos >= 0){
                    break;
                }else{
                    printf("\nIngreso de parametro d (ancho de discos) incorrecto!, debe ser mayor que cero\n"); //Verificacion pendiente
                    exit(EXIT_FAILURE);
                }
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
                }else{
                    printf("\nIngreso de parametro n (cantidad de discos) incorrecto!, debe ser mayor que cero\n"); //Verificacion pendiente
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
                }else{
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
    printf("\n i - %s",archivoVisibilidades);
    printf("\n o - %s",archivoSalida);
    printf("\n d - %d",anchoDiscos);
    printf("\n n - %d",cantDiscos);
    printf("\n s - %d",buffer);
    printf("\n b - %d",flag);
    
    //Inicializar estructura para resultados totales (comun)
    iniciarEstructuraComun();

    //MATRIZ ESTRUCTURA COMUN
    /*printf("\n");
    for(int i=0;i<cantDiscos;i++){
        for(int j=0;j<5;j++){
            printf("%lf",comun.resultadoTotalDiscos[i][j]);
            printf(" ");
        }
        printf("\n");
    }*/
    
	//Crear y Seteamos monitores
    printf("\n-----------\n");
	crearMonitores(listaMonitores);
    
    //MATRIZ ESTRUCTURA MONITOR
    printf("\n");
    for(int i=0;i<cantDiscos;i++){
        printf("%d %d %d",listaMonitores[i].indiceUltimo,listaMonitores[i].tamanoBUffer, listaMonitores[i].idMonitor);
        printf("\n");
    }

    
	//Crear y ejecutar hebras de discos
    pthread_t hebras[cantDiscos];
    for(int i= 0; i<cantDiscos;i++){
        pthread_create(&hebras[i],NULL,calculador, (void*)&listaMonitores[i]);
    }
    
	//Lectura de lineas y asignacion de variables al monitor
	asignarDataMonitores();

    /*se printea el seteo de la submatriz
    for(int i=0;i<cantDiscos;i++){
        printf("\n submatriz disco--> %d\n",listaMonitores[i].idMonitor);
		for(int j=0;j<buffer;j++){
			for(int k=0;k<5;k++){
                printf("%lf ", listaMonitores[i].subMatriz[j][k]);
			}
            printf("\n");
		}
	}*/
    
    //Juntar resultados en las hebras y mandarlo a la estructura de resultado total
    for(int i=0; i < cantDiscos;i++){
        pthread_join(hebras[i],NULL);
    }

	return 0;
}