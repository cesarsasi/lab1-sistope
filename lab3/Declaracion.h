#ifndef Declaracion_h_
#define Declaracion_h_
//Declaracion de estructuras a utilizar
typedef struct monitor{
	float **subMatriz;
	int indiceUltimo;
	int tamañoBUffer;
	int head, tail;
	int full, empty;
	pthread_mutex_t mutex;
	pthread_cond_t notFull, notEmpty;
} Monitor;
//falta estructura final
//Declaracion de variables globales a utilizar en el laboratorio
FILE *archivoEntrada;
char *mflag;
char* archivoVisibilidades,*archivoSalida;
int d;
int cantDiscos;
int buffer;
int anchoDiscos;
int flag;
Monitor *listaMonitores;
//Declaracion de funciones a utilizar en el laboratorio
void escrituraResultados(char * archivoFinal, int idDisco, int mediaReal, int mediaImag, int potencia, int ruidoTotal);
float calculoPotenciaParcial(float ** matriz, cantidad);
float calculoRuidoTotalParcial(float ** matriz, cantidad);
float calculoMediaReal(float matriz[3][5], int largo);
float calculoMediaImaginaria(float matriz[3][5], int largo);
void calculador();
void asignarDataMonitores(char * archivoVisibilidades, Monitor * listaMonitores):
void crearMonitores(Monitor * listaMonitores);

#endif