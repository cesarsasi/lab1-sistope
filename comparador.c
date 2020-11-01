//Laboratorio 1 
//Integrantes Octavio Canales - Cesar Salazar
//Sistemas Operativos 2.2020  (creacion - 01 de Noviembre 2020)

//Includes
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//Funciones
void programaComparador(FILE * archivo,char cadena[],int primeraLinea,int cantidadLineas,int identificador);

//Main programa comparador 
int main(int argc, char** argv){
    int pid = 0;
    int cantidadLineas= 0;
    int primeraLinea = 0;
    char cadena[] = "AAAA";
    FILE *fichero;       //puntero de fichero
    fichero = fopen("secuencia.txt", "r");
    programaComparador(fichero, cadena,primeraLinea, cantidadLineas, pid);
    fclose(fichero);
}

//Buscar Secuencia del tipo XXXX en una lista
//Casos borde: (fin)largo-3 || (linea menor a secuencia)largo < 4
void programaComparador(FILE * archivo,char cadena[],int primeraLinea,int cantidadLineas,int identificador){
    char linea[1024]; //almacenamiento previcional de la linea leida
    //Lee línea a línea (j es utilizado para leer el largo en la primera iteracion)
    int largo = 0; 
    while(fgets(linea, 1024, (FILE*) archivo) != NULL) {
        //Prueba ordenar cadena
        printf("--Linea--\n %s--Fin de la linea--\n", linea);

        //Obtencion del largo con la primera fila 
        int i=0;
        if (largo == 0){
            while (linea[i] != '\n'){
                largo= largo+1;
                i=i+1;
            }
            printf("Largo++++++++++++++++++++++++++++++++++++ %d",largo);
        }

        //Almacenamiento definitivo de las lineas


    }

    //Prueba pasar cadena 
    printf("%s\n", cadena );
}