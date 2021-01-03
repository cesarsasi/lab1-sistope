#ifndef Declaracion_h_
#define Declaracion_h_
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
Comun *comun;
char *mflag, *archivoVisibilidades, *archivoSalida;
int d,cantDiscos,buffer,anchoDiscos,flag;
int terminoLectura = 0;

//Declaracion de funciones a utilizar en el laboratorio
void escrituraResultados(char * archivoFinal, int idDisco, int mediaReal, int mediaImag, int potencia, int ruidoTotal);
float calculoPotenciaParcial(float ** matriz, int largo);
float calculoRuidoTotalParcial(float ** matriz, int largo);
float calculoMediaReal(float ** matriz, int largo);
float calculoMediaImaginaria(float ** matriz, int largo);
void *calculador(void * monitorVoid);
void asignarDataMonitores(char * archivoVisibilidades, Monitor * listaMonitores);
void crearMonitores(Monitor * listaMonitores);
void iniciarEstructuraComun();

#endif