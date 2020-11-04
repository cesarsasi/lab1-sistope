//Laboratorio 1 
//Integrantes Octavio Canales - Cesar Salazar
//Sistemas Operativos 2.2020  (creacion - 01 de Noviembre 2020)

//Includes
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define LECTURA 0
#define ESCRITURA 1


//Funciones
void guardarLineas(FILE * archivo,char * nombreArchivo,char cadena[],int primeraLinea,int cantidadLineas,int identificador);
int leerLargoLinea(char * archivo);
FILE *archivo;
FILE *fichero2;

//Main programa comparador 
int main(int argc, char** argv){

    int pid=getpid();
    int cantidadLineas;
    int primeraLinea;
    char instruccionesREC[60];

    read(STDIN_FILENO,instruccionesREC,60*sizeof(char));
    //seprar variables
    printf("%s\n", instruccionesREC);
    char * instruccion = strtok(instruccionesREC,",");
    char * archivoSTR = instruccion;
    printf("---%s\n", archivoSTR);
    instruccion = strtok(NULL,",");
    char * primeralineaSTR = instruccion;
    printf("---%s\n", primeralineaSTR);
    primeraLinea = atoi(primeralineaSTR);
    instruccion = strtok(NULL,",");
    char * cadena = instruccion;
    printf("---%s\n", cadena);
    instruccion = strtok(NULL,",");
    char * cantidadLineasSTR = instruccion;
    printf("---%s\n", cantidadLineasSTR);
    cantidadLineas = atoi(cantidadLineasSTR);

    archivo = fopen(archivoSTR, "r");
    guardarLineas(archivo, archivoSTR, cadena, primeraLinea, cantidadLineas, pid);
    fclose(archivo);
}

//Buscar Secuencia del tipo XXXX en una lista
//Casos borde: (fin)largo-3 || (linea menor a secuencia)largo < 4
void guardarLineas(FILE * archivo,char * nombreArchivo,char cadena[],int primeraLinea,int cantidadLineas,int identificador){
    //NOMbre
    char rp[100]="rp_";
    strcat(rp,cadena);
    char pid[100];
    sprintf(pid,"%d",identificador);
    strcat(rp,"_");
    strcat(rp,pid);
    strcat(rp,".txt");

    //Se crea
    FILE *salida;
    salida=fopen(rp,"w");
    fclose(salida);

    //Almacenar 
    int largo;
    largo = leerLargoLinea(nombreArchivo);
    printf("\n largo linea %d \n",largo);
    char matrizArchivo[cantidadLineas][largo+1];
    //Cadena que almacenare momentaneamente las lineas que no correspondan a revisar
    char lineasX[largo+1];
    //Dejo el puntero hasta la linea que se desea comenzar a leer
    for(int i=0; i<primeraLinea; i++){
        fgets(lineasX,largo+1,archivo);
    }
    if(primeraLinea != 0){
        fgets(lineasX,largo+1,archivo);
    }
    //Hago el almacenamiento de las lineas necesarias
    for(int i = 0; i < cantidadLineas; i++){
        for(int j=0 ; j < largo+1; j++){
            fscanf(archivo, "%c", &matrizArchivo[i][j]);
        }
    }

    //Prueba print matriz
    for(int i =0 ; i < cantidadLineas; i++){
        for(int j=0 ; j < largo+1; j++){
            printf("%c", matrizArchivo[i][j]);
        }
        printf("\n");
    }

    //Escritura
    int i=0;
    int j=0;
    int match = 0;
    while(i != cantidadLineas){
        while(j != largo-4){
            if(cadena[0]==matrizArchivo[i][j] && cadena[1]==matrizArchivo[i][j+1] && cadena[2]==matrizArchivo[i][j+2] && cadena[3]==matrizArchivo[i][j+3]){
                match++;
            }
            j++;
        }
        if(match != 0){
            printf("\n Match fila %d", i+1);
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
        }else{
            printf("\n No Match fila %d", i+1);

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

int leerLargoLinea(char * archivo){
    fichero2 = fopen(archivo, "r");
    //Obtencion del largo con la primera fila 
    int largoLineas=0;
    char caracter;
    while((caracter = fgetc(fichero2)) != '\n'){
        largoLineas=largoLineas+1;
    }
    fclose(fichero2);
    return largoLineas;
}