/*
Laboratorio 1
Profesor   : Rannou
Ayudante   : Benjamin
Integrantes:    Octavio Canales 
                César Antonio Salazar Silva  19.916.471-6
Sistemas Operativos 2.2020  (Creación: 01-Noviembre-2020)
*/

//Se extienden librerias
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define LECTURA 0
#define ESCRITURA 1

//Inicializar punteros dedicados a los archivos a manejar.
FILE *archivo;
FILE *fichero2;

//Declaracion de funciones
void comparar(FILE * archivo,char * nombreArchivo,char cadena[],int primeraLinea,int cantidadLineas,int identificador);
int leerLargoLinea(char * archivo);

//BLOQUE PRINCIPAL - FUNCIONALIDAD COMPARADOR
int main(int argc, char** argv){
    //Se obtiene el pid del proceso para escribirlo en el nombre del archivo
    int pid=getpid();
    //Inicializar cariables utilizadas como parametros recibidos para comparar
    int cantidadLineas, primeraLinea;
    char instruccionesREC[60];
    //Leer el string con los datos pasados por el pipe (nombre,primeraLinea,cadenaABuscar,cantLineas)
    read(STDIN_FILENO,instruccionesREC,60*sizeof(char));
    //Separar variables del String con strtok y luego convertirlos en el tipo de dato requerido
    char * instruccion = strtok(instruccionesREC,",");
    char * archivoSTR = instruccion;
    instruccion = strtok(NULL,",");
    char * primeralineaSTR = instruccion;
    primeraLinea = atoi(primeralineaSTR);
    instruccion = strtok(NULL,",");
    char * cadena = instruccion;
    instruccion = strtok(NULL,",");
    char * cantidadLineasSTR = instruccion;
    cantidadLineas = atoi(cantidadLineasSTR);
    //Abrir el archivo entregado y comenzar con la comparacion solicitada por el coordinador
    archivo = fopen(archivoSTR, "r");
    comparar(archivo, archivoSTR, cadena, primeraLinea, cantidadLineas, pid);
    fclose(archivo);
}

//Funcion comparador, encargado de buscar Secuencia del tipo XXXX en una lista
//Entrada : Puntero del archivo a leer,El nombre del archivo a leer[Arreglo unidimensional de caracteres], ...
//          cadena a buscar[Arreglo unidimensional de caracteres], primera linea a leer[Entero], cantidad de lineas a leer[Entero]
//Salida  : Ninguna, se modifican archivos en esta funcion.
//Casos borde: (fin)largo-3 || (linea menor a secuencia)largo < 4
void comparar(FILE * archivo,char * nombreArchivo,char cadena[],int primeraLinea,int cantidadLineas,int identificador){
    //Se forma el nombre del archivo parcial en el que se guardará el contenido parcial
    char rp[100]="archivos/rp_";
    strcat(rp,cadena);
    char pid[100];
    sprintf(pid,"%d",identificador);
    strcat(rp,"_");
    strcat(rp,pid);
    strcat(rp,".txt");
    //Se inicializa el archivo para el resultado parcial
    FILE *salida;
    salida=fopen(rp,"w");
    fclose(salida);
    //Almacenamiento del contenido a comparar
    int largo;
    largo = leerLargoLinea(nombreArchivo);
    char matrizArchivo[cantidadLineas][largo+1];
    //Cadena que almacenare momentaneamente las lineas que no correspondan a revisar
    char lineasX[largo+1];
    //Dejo el puntero hasta la linea que se desea comenzar a leer
    //Si primeraLinea es 0 no se hace nada, pero si es mayor al terminar de posicionar el puntero se hace getc
    for(int i=0; i<primeraLinea; i++){
        fgets(lineasX,largo+2,archivo);
    }
    //Hago el almacenamiento de las lineas necesarias
    for(int i =0 ; i < cantidadLineas; i++){
        for(int j=0 ; j < largo+1; j++){
            fscanf(archivo, "%c", &matrizArchivo[i][j]);
        }
    }
    //Buscar match y escribir resultado
    int i=0;
    int j=0;
    int match = 0;
    while(i != cantidadLineas){
        //Si coinciden la secuencia con "XXXX" cadena solicitada dentro de la linea, aumenta el match (contador) en uno
        while(j != largo-4){
            if(cadena[0]==matrizArchivo[i][j] && cadena[1]==matrizArchivo[i][j+1] && cadena[2]==matrizArchivo[i][j+2] && cadena[3]==matrizArchivo[i][j+3]){
                match++;
            }
            j++;
        }
        //Con match mayor a cero se imprime la linea y que hubo coincidencia
        if(match != 0){
            FILE *salida;
            salida= fopen(rp,"a");
            for(int k =i ; k <= i; k++){
                for(int m=0 ; m < largo; m++){
                    fprintf(salida,"%c", matrizArchivo[k][m]);
                }
                fprintf(salida, "  ");
                fprintf(salida, "SI\n");
            }
            fclose(salida);
        //Con match menor a cero se imprime la linea y que no hubo coincidencia
        }else{
            FILE *salida;
            salida= fopen(rp,"a");
            for(int k =i ; k <= i; k++){
                for(int m=0 ; m < largo; m++){
                    fprintf(salida,"%c", matrizArchivo[k][m]);
                }
                fprintf(salida, "  ");
                fprintf(salida, "NO\n");               
            }
            fclose(salida);
        }
        match = 0;
        j = 0;
    i++;
    }
}

//Funcion que encarga de abrir un archivo y leer el largo de la primera linea
//Entrada : El nombre del archivo [Arreglo unidimensional de caracteres].
//Salida  : Cantidad de caracteres contenidos en la primera linea [Entero].
int leerLargoLinea(char * archivo){
    //Apertura del archivo
    fichero2 = fopen(archivo, "r");
    //Iniciar contador de caracteres
    int largoLineas=0;
    char caracter;
    //Recorrer primera linea (hasta salto de linea)
    while((caracter = fgetc(fichero2)) != '\n'){
        largoLineas=largoLineas+1;
    }
    //Cerrar el fichero
    fclose(fichero2);
    return largoLineas;
}