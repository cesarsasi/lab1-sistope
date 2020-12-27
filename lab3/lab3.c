#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>



FILE *archivoEntrada;

// ./lab3 -i prueba1.csv -o propiedades.txt -d 100 -n 4 -s 10 -b
int main(int argc, char** argv){
	char *mflag;
	char* archivoVisibilidades,*archivoSalida;
	int d;
	int cantDiscos = 0;
	int buffer = 0;
	int anchoDiscos = 0;
	int flag;
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

	//-----------------------------------------------------------------------------
	int largo = 0;
	archivoEntrada=fopen(archivoVisibilidades,"r");
	char c;
	int datos;
	while((c = fgetc(archivoEntrada)) != EOF){
		if(c == '\n'){
			largo++;
		}
	}
	fclose(archivoEntrada);
	printf("\n largo archivo %d \n", largo);
	float **archivoGuardado = (float **)calloc(sizeof(float*), largo);
	for(int i=0; i<largo;i++){
		archivoGuardado[i]=(float*)calloc(sizeof(float),5);
	}
	archivoEntrada=fopen(archivoVisibilidades,"r");
	for(int i=0; i<largo;i++){
		for(int j=0;j<5;j++){
			fscanf(archivoEntrada, "%f", &archivoGuardado[i][j]);
			c=fgetc(archivoEntrada);
		}
	}
	fclose(archivoEntrada);

	for(int i=0;i<largo;i++){
        for(int j=0;j<5;j++){
            printf("%f",archivoGuardado[i][j]);
            printf(" ");
        }
        printf("\n");
    }

	return 0;
}