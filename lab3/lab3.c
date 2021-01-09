#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<pthread.h>
#include<stdbool.h>

#include "Declaracion.h"
/* FUNCION : Principal, gestiona el llamado de la lectura y a su vez del productor
ENTRADA: Parametros ingresados por usuario al momento de ejecutar el programa, tales como (ver observaciones).
SALIDA : Retorno a 0 al terminar el programa
OBS    :
// ./lab3 -i prueba1.csv -o propiedades.txt -d 100 -n 4 -s 10 -b
*/
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
    //Inicializar estructura para resultados totales (comun)
    iniciarEstructuraComun();
	//Crear y Seteamos monitores
	crearMonitores(listaMonitores);
	//Crear y ejecutar hebras de discos
    pthread_t hebras[cantDiscos];
    for(int i= 0; i<cantDiscos;i++){
        pthread_create(&hebras[i],NULL,calculador, (void*)&listaMonitores[i]);
    }
	//Lectura de lineas y asignacion de variables al monitor
	asignarDataMonitores();
    //Juntar resultados en las hebras y mandarlo a la estructura de resultado total
    for(int i=0; i < cantDiscos;i++){
        pthread_join(hebras[i],NULL);
    }
    if (flag == 1){
        for(int i=0;i<cantDiscos;i++){
            printf("\nLineas leidas por el disco %d --> %lf\n",listaMonitores[i].idMonitor,listaMonitores[i].lineasLeidas);
        }
    }
	return 0;
}