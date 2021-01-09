#ifndef Declaracion_h_
#define Declaracion_h_
//Declaracion de estructuras a utilizar
//DECLARACION-------------------------------------------------------------------------------------------------------------------------
//Declaracion de estructuras a utilizar
typedef struct monitor{
	double **subMatriz;
	double ** resultadoTotalDiscos;
	int indiceUltimo;
	double lineasLeidas;
	int tamanoBUffer;
	int idMonitor;
	int full, empty;
	pthread_mutex_t mutex;
	pthread_cond_t notFull, notEmpty;
} Monitor;

typedef struct comun{
	double ** resultadoTotalDiscos; //Estructura del arreglo [rD1,rD2,.....n] -> (rD1 = [mR,mI,P,R])
}Comun;

//Declaracion de variables globales a utilizar en el laboratorio
FILE *archivoEntrada;
Monitor *listaMonitores;
Comun Ecomun;
char *mflag, *archivoVisibilidades, *archivoSalida;
int d,cantDiscos,buffer,anchoDiscos,flag;
int terminoLectura;


//Declaracion de funciones a utilizar en el laboratorio
void escrituraResultados(char * archivoFinal, int idDisco, int mediaReal, int mediaImag, int potencia, int ruidoTotal);
double calculoPotenciaParcial(double ** matriz, int largo);
double calculoRuidoTotalParcial(double ** matriz, int largo);
double calculoSumaMediaReal(double ** matriz, int largo);
double calculoSumaMediaImaginaria(double ** matriz, int largo);
void *calculador(void * monitorVoid);
void asignarDataMonitores();
void crearMonitores();
void iniciarEstructuraComun();
//DECLARACION-------------------------------------------------------------------------------------------------------------------------
#endif
