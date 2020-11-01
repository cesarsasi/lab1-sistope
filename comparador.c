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

FILE *archivo;
FILE *fichero2;
//Main programa comparador 
int main(int argc, char** argv){
    int pid = 0;
    int cantidadLineas= 4;
    int primeraLinea = 0;
    char cadena[] = "AAAA";

     //puntero de fichero
    archivo = fopen("secuencia.txt", "r");

    programaComparador(archivo, cadena,primeraLinea, cantidadLineas, pid);
    fclose(archivo);
}

//Buscar Secuencia del tipo XXXX en una lista
//Casos borde: (fin)largo-3 || (linea menor a secuencia)largo < 4
void programaComparador(FILE * archivo,char cadena[],int primeraLinea,int cantidadLineas,int identificador){
    int largo;
    largo = leerLargoLinea();
    printf("\n largo linea %d \n",largo);
    char matrizArchivo[cantidadLineas][largo+1];
    for(int i =0 ; i < cantidadLineas; i++){
        for(int j=0 ; j < largo+1; j++){
            fscanf(archivo, "%c", &matrizArchivo[i][j]);
        }
    }
    for(int i =0 ; i < cantidadLineas; i++){
        for(int j=0 ; j < largo+1; j++){
            printf("%c", matrizArchivo[i][j]);
        }
        printf("\n");
    }
}

int leerLargoLinea(){
    fichero2 = fopen("secuencia.txt", "r");
    //Obtencion del largo con la primera fila 
    int largoLineas=0;
    char caracter;
    while((caracter = fgetc(fichero2)) != '\n'){
        largoLineas=largoLineas+1;
    }
    fclose(fichero2);
    return largoLineas;
}