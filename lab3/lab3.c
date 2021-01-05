#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<pthread.h>
#include<stdbool.h>

//#include "Declaracion.h"

//DECLARACION-------------------------------------------------------------------------------------------------------------------------
//Declaracion de estructuras a utilizar
typedef struct monitor{
	float **subMatriz;
	int indiceUltimo;
	int tamanoBUffer;
	int idMonitor;
	int full, empty;
	pthread_mutex_t mutex;
	pthread_cond_t notFull, notEmpty;
} Monitor;

typedef struct comun{
	float ** resultadoTotalDiscos; //Estructura del arreglo [rD1,rD2,.....n] -> (rD1 = [mR,mI,P,R])
}Comun;

//Declaracion de variables globales a utilizar en el laboratorio
FILE *archivoEntrada;
Monitor *listaMonitores;
Comun comun;
char *mflag, *archivoVisibilidades, *archivoSalida;
int d,cantDiscos,buffer,anchoDiscos,flag;
int terminoLectura ;


//Declaracion de funciones a utilizar en el laboratorio
void escrituraResultados(char * archivoFinal, int idDisco, int mediaReal, int mediaImag, int potencia, int ruidoTotal);
float calculoPotenciaParcial(float ** matriz, int largo);
float calculoRuidoTotalParcial(float ** matriz, int largo);
float calculoMediaReal(float ** matriz, int largo);
float calculoMediaImaginaria(float ** matriz, int largo);
void *calculador(void * monitorVoid);
void asignarDataMonitores();
void crearMonitores();
void iniciarEstructuraComun();
//DECLARACION-------------------------------------------------------------------------------------------------------------------------





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
    pthread_t *hebras;
    
    printf();
    //Inicializar estructura para resultados totales (comun)
    //iniciarEstructuraComun();
    printf("\n HOLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");/*
	comun.resultadoTotalDiscos = (float**)calloc(sizeof(float*),cantDiscos);
    for (int i = 0; i < cantDiscos; i++){
        comun.resultadoTotalDiscos[i] = (float*)calloc(sizeof(float),5);
		for (int j = 0; j < 5; j++){
			comun.resultadoTotalDiscos[i][j]= 0;
		}
    }*/
    
	//Crear y Seteamos monitores
	crearMonitores(listaMonitores);

	//Crear y ejecutar hebras de discos
    
    hebras = (pthread_t*)calloc(sizeof(pthread_t),cantDiscos);
    for(int i= 0; i<cantDiscos;i++){
        pthread_create(&hebras[i],NULL,calculador, &listaMonitores[i]);
    }

	//Lectura de lineas y asignacion de variables al monitor
	asignarDataMonitores(archivoVisibilidades,listaMonitores);

    //Juntar resultados en las hebras y mandarlo a la estructura de resultado total
    for(int i=0; i < cantDiscos;i++){
        pthread_join(hebras[i],NULL);
    }

	return 0;
}


//FUNCIONES-------------------------------------------------------------------------------------------------------------------------
void escrituraResultados(char * archivoFinal, int idDisco, int mediaReal, int mediaImag, int potencia, int ruidoTotal){
    FILE* fileFinal = fopen(archivoFinal, "a");
    fprintf(fileFinal,"Disco: %d",idDisco);
    fprintf(fileFinal,"Media real: %d",mediaReal);
    fprintf(fileFinal,"Media imaginaria: %d",mediaImag);
    fprintf(fileFinal,"Potencia: %d",potencia);
    fprintf(fileFinal,"Ruido total %d:",ruidoTotal);
}

float calculoPotenciaParcial(float ** matriz, int largo){
    float potenciaParcial = 0;
    for (int i = 0; i < largo; ++i){
        potenciaParcial += pow(matriz[i][2],2) + sqrt(pow(matriz[i][3],2));
    }
    return potenciaParcial;
}

float calculoRuidoTotalParcial(float ** matriz, int largo){
    float ruidoTotalParcial = 0;
    for (int i = 0; i < largo; ++i){
        ruidoTotalParcial += matriz[i][4];
    }
    return ruidoTotalParcial;
    
}

float calculoMediaReal(float ** matriz, int largo){
    float sumaReal=0.0;
    float largoF= (float)largo;
    float mediaR;
    for(int i=0;i<largo;i++){
        sumaReal=sumaReal+matriz[i][2];
    }
    mediaR = sumaReal/largoF;
    return mediaR;
}

float calculoMediaImaginaria(float ** matriz, int largo){
    float sumaImaginaria =0.0;
    float largoF=(float)largo;
    float mediaI;
    for(int i=0;i<largo;i++){
        sumaImaginaria=sumaImaginaria+matriz[i][3];
    }
    mediaI= sumaImaginaria/largoF;
    return mediaI;
}
//*******************************************************************************************************************************
//Calculador reune las funciones.h y las ejecuta 
void * calculador(void * monitorVoid){
	
	Monitor * monitor = (Monitor *)monitorVoid;
	while(true){
		//si ya esta tomado el mutex la hebra se bloquea
		pthread_mutex_lock (&monitor->mutex);
		//si el monitor no llena buffer no se ejecuta lo que continua despues del while
		while (monitor->empty) {
			pthread_cond_wait (&monitor->notEmpty, &monitor->mutex);
		}
		// el buffer esta lleno, por lo tanto se ejecuta la logica del lab
		float resultadoParcial[4];
		resultadoParcial[0] = calculoPotenciaParcial(monitor->subMatriz, monitor->indiceUltimo);
		resultadoParcial[1] = calculoRuidoTotalParcial(monitor->subMatriz, monitor->indiceUltimo);
		resultadoParcial[2] = calculoMediaReal(monitor->subMatriz, monitor->indiceUltimo); //------------------------------------------Modificar a calculo real!!!!!
		resultadoParcial[3] = calculoMediaImaginaria(monitor->subMatriz, monitor->indiceUltimo);//-------------------------------------Modificar a calculo real!!!!!
		//Escribir en la estructura correspondiente y guardar en el monitor con la estructura Manejo parcial
		for (int i = 0; i < 5; i++){
			comun.resultadoTotalDiscos[monitor->idMonitor][i] += resultadoParcial[i];
		}
		//Vaciar submatriz y reestablecer los datos del monitor
		for (int i = 0; i < buffer; i++){
			//free(monitor->subMatriz[i]);
		}
		//free(monitor->subMatriz);
		monitor->indiceUltimo = 0;

		//Liberar la hebra
		pthread_cond_signal(&monitor->notFull);
		pthread_mutex_unlock(&monitor->mutex);
	}
}

void asignarDataMonitores(){
    int largo = 0;
	archivoEntrada=fopen(archivoVisibilidades,"r");
	char c;
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
	float posU, posV,posR,posI,posRU;
	archivoEntrada=fopen(archivoVisibilidades,"r");
	while(true){

		for(int i=0; i<largo;i++){
			for(int j=0;j<5;j++){
				fscanf(archivoEntrada, "%f", &posU);
				c=fgetc(archivoEntrada);

				fscanf(archivoEntrada, "%f", &posV);
				c=fgetc(archivoEntrada);
				
				fscanf(archivoEntrada, "%f", &posR);
				c=fgetc(archivoEntrada);

				fscanf(archivoEntrada, "%f", &posI);
				c=fgetc(archivoEntrada);

				fscanf(archivoEntrada, "%f", &posRU);
				c=fgetc(archivoEntrada);
				
				//Productor calcula propiedades del consumidor y consumidor es el que procesa
				//Distribucion y como identificar cuando un punto va a un monitor
				
				//Obtenemos su distancia del centro}
				float sumPot = pow(posU,2) + pow(posV,2);
				float dist = sqrt(sumPot);
				//calculamos en que disco queda asignada (indice del monitor en el arreglo)
				int indiceDiscAsignado = -1;
				for (int j = 0; j < cantDiscos; ++j){
					if( (anchoDiscos*j)<=dist && (anchoDiscos*j)+anchoDiscos>dist){
						indiceDiscAsignado = j;
					}
				}
				if (indiceDiscAsignado == -1){
					indiceDiscAsignado = cantDiscos -1;
				}
				//con este for identificamos el monitor que debemos usar
				if(listaMonitores[indiceDiscAsignado].indiceUltimo < buffer){
					listaMonitores[indiceDiscAsignado].subMatriz[listaMonitores->indiceUltimo][0]=posU;
					listaMonitores[indiceDiscAsignado].subMatriz[listaMonitores->indiceUltimo][1]=posV;
					listaMonitores[indiceDiscAsignado].subMatriz[listaMonitores->indiceUltimo][2]=posR;
					listaMonitores[indiceDiscAsignado].subMatriz[listaMonitores->indiceUltimo][3]=posI;
					listaMonitores[indiceDiscAsignado].subMatriz[listaMonitores->indiceUltimo][4]=posRU;
					listaMonitores->indiceUltimo+=1;
				}
				// se termina la produccion
				if(listaMonitores[indiceDiscAsignado].indiceUltimo == buffer){//el buffer queda lleno
					//bloqueamos el mutex del monitor
					pthread_mutex_lock (&listaMonitores[indiceDiscAsignado].mutex);
					//mientras este lleno se bloquea
					while (listaMonitores[indiceDiscAsignado].full) {
						pthread_cond_wait (&listaMonitores[indiceDiscAsignado].notFull, &listaMonitores[indiceDiscAsignado].mutex);
					}
					//se libera el monitor correspondiente que ya previamente vacia el buffer del monitor y permite proceder la lectura
					pthread_cond_signal(&listaMonitores[indiceDiscAsignado].notEmpty);
					pthread_mutex_unlock(&listaMonitores[indiceDiscAsignado].mutex);
				}	
				
				
			}
		}
	//Leer lineas que quedan washitas
	}
	fclose(archivoEntrada);

}

void crearMonitores(){
    //crear monitor
    listaMonitores = (Monitor*)calloc(sizeof(Monitor),cantDiscos);
    for(int i=0;i<cantDiscos;i++){
    	pthread_mutex_init(&listaMonitores[i].mutex, NULL);
    	pthread_cond_init(&listaMonitores[i].notFull, NULL);
    	pthread_cond_init(&listaMonitores[i].notEmpty, NULL);
    	listaMonitores[i].indiceUltimo=0;
    	listaMonitores[i].tamanoBUffer=buffer;
		listaMonitores[i].idMonitor=i+1;
    	listaMonitores[i].subMatriz=(float**)calloc(sizeof(float*),buffer);
    	for(int j=0;j<5;j++){
    		listaMonitores[i].subMatriz[j]=(float*)calloc(sizeof(float),5);
    	}
    }
}


/*void iniciarEstructuraComun(){
	printf("\n HOLAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	comun.resultadoTotalDiscos = (float**)calloc(sizeof(float*),cantDiscos);
    for (int i = 0; i < cantDiscos; i++){
        comun.resultadoTotalDiscos[i] = (float*)calloc(sizeof(float),5);
		for (int j = 0; j < 5; j++){
			comun.resultadoTotalDiscos[i][j]= 0;
		}
    }
}*/


//Imprimir matriz Prueba
	/*for(int i=0;i<largo;i++){
        for(int j=0;j<5;j++){
            printf("%f",archivoGuardado[i][j]);
            printf(" ");
        }
        printf("\n");
    }*/ 
//FUNCIONES-------------------------------------------------------------------------------------------------------------------------