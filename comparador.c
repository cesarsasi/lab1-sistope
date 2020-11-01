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
int leerLargoLinea();

//Main programa comparador 
int main(int argc, char** argv){
    int pid = 0;
    int cantidadLineas= 4;
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
    int contLineasAlmac = 0;
    //Lee línea a línea (j es utilizado para leer el largo en la primera iteracion)
    int largoLineas = leerLargoLinea(); 
    printf("\n Largo = %d \n",largoLineas);
    char linea[largoLineas];
    char * lineasAlmacenadas[5]; 
    int contLineas = 0;
    
    while(fgets(linea, largoLineas+1, (FILE*) archivo) != NULL) {
        //Prueba ordenar cadena
        //printf("--Linea--\n %s--Fin de la linea--\n", linea);

        //Almacenamiento definitivo de las lineas (condición limite de largos) *****IMPORTANTE: PROGRAMA COORDINADOR TOMA LA PRIMERA LINEA COMO 0
        if(contLineas >= primeraLinea && contLineas < primeraLinea + cantidadLineas){
            lineasAlmacenadas[contLineasAlmac] = linea;
            contLineasAlmac++;
        }     // 0-4       primeralinea 0 --- cantidadlineas 5 ------   4< 0+5 
        //

        contLineas++;
    }
    //Prueba Almac
    for (int contPrueba = 0; contPrueba < cantidadLineas; contPrueba++){
             printf("--Linea--\n %s--Fin de la linea--\n", lineasAlmacenadas[contPrueba]);
        }

    //Prueba pasar cadena 
    printf("%s\n", cadena );
}

int leerLargoLinea(){
    FILE *fichero;       //puntero de fichero
    fichero = fopen("secuencia.txt", "r");
    //Obtencion del largo con la primera fila 
    int largoLineas;
    int i=0;
    char linea[1024];
    fgets(linea, 1024, (FILE*) fichero);
    if (largoLineas == 0){
        while (linea[i] != '\n'){
            largoLineas= largoLineas+1;
            i=i+1;
        }
    }
    fclose(fichero);
    return largoLineas;
}