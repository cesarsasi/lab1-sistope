/*
Laboratorio 3
Profesor   : Fernando Rannou
Ayudante   : Benjamin Muñoz
Integrantes:
-                Octavio Nicolas Canales Ñirriman 20.003.610-7
-                César Antonio Salazar Silva      19.916.471-6
Sistemas Operativos 2.2020  (Creación: 26 de Diciembre del 2020)
*/
#ifndef Declaracion_h_
#define Declaracion_h_
// ESTRUCTURA que simula monitor, contiene los parametros que permiten el manejo corrector del traspaso de informacion entre hebras
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

//ESTRUCTURA que almacena los resultados finales de los diascos, segun el manejo de cada monitor.
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
void escrituraResultados();
double calculoPotenciaParcial(double ** matriz, int largo);
double calculoRuidoTotalParcial(double ** matriz, int largo);
double calculoSumaMediaReal(double ** matriz, int largo);
double calculoSumaMediaImaginaria(double ** matriz, int largo);
void *calculador(void * monitorVoid);
void asignarDataMonitores();
void crearMonitores();
void iniciarEstructuraComun();
#endif
