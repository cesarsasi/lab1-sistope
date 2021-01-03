#ifndef funcionesVarias_h_
#define funcionesVarias_h_

#include "lab3.c"


void lecturaParametros(int argc, char** argv){
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
}

void escrituraResultados(char * archivoFinal, int idDisco, int mediaReal, int mediaImag, int potencia, int ruidoTotal){
    FILE* fileFinal = fopen(archivoFinal, "a");
    fprintf(fileFinal,"Disco: %d",idDisco);
    fprintf(fileFinal,"Media real: %d",mediaReal);
    fprintf(fileFinal,"Media imaginaria: %d",mediaImag);
    fprintf(fileFinal,"Potencia: %d",potencia);
    fprintf(fileFinal,"Ruido total %d:",ruidoTotal);
}

float calculoPotenciaParcial(float ** matriz, cantidad){
    float potenciaParcial = 0;
    for (int i = 0; i < cantidad; ++i){
        potenciaParcial += pow(matriz[i][2],2) + sqrt(pow(matriz[i][3],2));
    }
    return potenciaParcial;
}

float calculoRuidoTotalParcial(float ** matriz, cantidad){
    float ruidoTotalParcial = 0;
    for (int i = 0; i < cantidad; ++i){
        potenciaParcial += matriz[i][4];
    }
    return potenciaParcial;
    
}

float calculoMediaReal(float matriz[3][5], int largo){
    float sumaReal=0.0;
    float largoF= (float)largo;
    float mediaR;
    for(int i=0;i<largo;i++){
        sumaReal=sumaReal+matriz[i][2];
    }
    mediaR = sumaReal/largoF;
    return mediaR;
}
float calculoMediaImaginaria(float matriz[3][5], int largo){
    float sumaImaginaria =0.0;
    float largoF=(float)largo;
    float mediaI;
    for(int i=0;i<largo;i++){
        sumaImaginaria=sumaImaginaria+matriz[i][3];
    }
    mediaI= sumaImaginaria/largoF;
    return mediaI;
}
/*
0 u
1 v
2 r
3 i
4 w */
#endif