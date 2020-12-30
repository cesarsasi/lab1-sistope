#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <funciones1.h>


FILE *archivoEntrada;
typedef struct monitor{
	float **subMatriz;
	int indiceUltimo;
	int tamañoBUffer;
	int head, tail;
	int full, empty;
	pthread_mutex_t mutex;
	pthread_cond_t notFull, notEmpty;
} Monitor;
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
    //crear y ejecutar hebras de discos
    pthread_t* hebras;
    hebras = (pthread_t)calloc(sizeof(pthread_t),cantDiscos);
    //crear monitor
    Monitor *listaMonitores= (Monitor)calloc(sizeof(Monitor),cantDiscos);
    for(int i=;i<cantDiscos;i++){
    	pthread_mutex_init(&listaMonitores[i]->mutex, NULL);
    	pthread_cond_init(&listaMonitores[i]->notFull, NULL);
    	pthread_cond_init(&listaMonitores[i]->notEmpty, NULL);
    	&listaMonitores[i]->indiceUltimo=0;
    	&listaMonitores[i]->tamañoBUffer=buffer;
    	&listaMonitores[i]->subMatriz=(float**)calloc(sizeof(float*),buffer);
    	for(int j=0;j<5;j++){
    		&listaMonitores[i]->subMatriz[j]=(float*)calloc(sizeof(float),5);
    	}
    }
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

    //Productor calcula propiedades del consumidor y consumidor es el que calcula todu
    //Distribucion y como identificar cuando un punto va a un monitor
    for(int i = 0; i < largo; i ++){
    	//Leemos la linea en particular
    	float posU = archivoGuardado[i][0];
		float posV = archivoGuardado[i][1];
    	//Obtenemos su distancia del centro}
    	float sumPot = pow(posU,2) + pow(posV,2);
    	float dist = sqrt(sumPot);
    	//calculamos en que disco queda asignada (indice del monitor en el arreglo)
    	int indiceDiscAsignado = -1;
    	for (int j = 0; j < cantDiscos; ++j){
    		if( rangosDisco[j]<dist && rangosDisco[j]+anchoDiscos>dist){
    			indiceDiscAsignado = j;
    		}
    	}
    	if(indiceDiscAsignado != -1){
    		//con este for identificamos el monitor que debemos usar
    		if(&listaMonitores->indiceUltimo < buffer){
	    		&listaMonitores[indiceDiscAsignado]->subMatriz[&listaMonitores->indiceUltimo][0]=archivoGuardado[i][0];
	    		&listaMonitores[indiceDiscAsignado]->subMatriz[&listaMonitores->indiceUltimo][1]=archivoGuardado[i][1];
	    		&listaMonitores[indiceDiscAsignado]->subMatriz[&listaMonitores->indiceUltimo][2]=archivoGuardado[i][2];
	    		&listaMonitores[indiceDiscAsignado]->subMatriz[&listaMonitores->indiceUltimo][3]=archivoGuardado[i][3];
	    		&listaMonitores[indiceDiscAsignado]->subMatriz[&listaMonitores->indiceUltimo][4]=archivoGuardado[i][4];
	    		&listaMonitores->indiceUltimo+=1;
    		}
    		//el buffer queda lleno
    		else{
    			//ejecutamos el proceso y hacemos que la hebra se ejecute haciendo esperar la lectura
    			pthread_create(&hebra[indiceDiscAsignado], NULL, calculador, (void *) &listaMonitores[indiceDiscAsignado]);
				//se vacia el buffer del monitor y continua la lectura
    		}	
    	}
    }

    //Juntar resultados en las hebras y mandarlo a la estructura de resultado total

    //Printear o escribir en documento

	return 0;
}