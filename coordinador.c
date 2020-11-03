//Includes
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t


#define ESCRITURA 1
#define LECTURA 0
//Funciones
typedef struct instrucciones instrucciones;
struct instrucciones{
    char* archivoEntrada;
    int lineaInicia;
    char *cadenaBuscar;
    int lineasporProcesos;
};
//Main programa coordinador
int main(int argc, char** argv){
	

	int numeroProcesos,lineasArchivo;
    char* archivoEntrada,*cadenaBuscar;
    int flag;
    char *mflag;
    int c;
    char* ejecutableExec[2] = {"./comparador", NULL};
    int *pipes = (int*)malloc(sizeof(int)*2);
    pipe(pipes); //inicializa el pipe

    while (( (c = getopt(argc, argv, "i:n:c:p:d")) != -1)){
        switch (c)
        {
        case 'i':
			//./ejecutable -i archivo.txt
            mflag = optarg; 
			if(optarg == 0){
				printf("\nIngreso de parametro i incorrecto! \n");
                exit(EXIT_FAILURE);
			}else{
				archivoEntrada = optarg;
			}
        	break;

        case 'n':
			//./ejecutable -i archivo.txt
            mflag = optarg; 
            if(optarg == 0){
				printf("\nIngreso de parametro n incorrecto!\n");
                exit(EXIT_FAILURE);
			}else{
				numeroProcesos = atof(optarg);
			}
        	break;

		case 'c':
			//./ejecutable -i archivo.txt
			mflag = optarg; 
            if(optarg == 0){
				printf("\nIngreso de parametro c incorrecto!\n");
                exit(EXIT_FAILURE);
			}else{
				lineasArchivo = atof(optarg);
			}
        	break;			
    
        case 'p':
			//./ejecutable -i archivo.txt
			if(optarg == 0){
				printf("\nIngreso de parametro p incorrecto!\n");
                exit(EXIT_FAILURE);
			}else{
				cadenaBuscar = optarg;
			}
        	break;

        case 'd':
			flag = 1;
			break;
        }
    }
    printf("el archivo de entrada es: %s\n",archivoEntrada);
    printf("el numero de procesos es: %d\n",numeroProcesos);
	printf("el numero de lineas es: %d\n",lineasArchivo);
    printf("la cadena ingresada es: %s\n",cadenaBuscar);

	if (flag != 0){
		printf("Hay Banderita!");
	}
	pid_t pid;

	//Programa coordinador                            Procesos no puede ser 0 !!!!!!!
    //Proceso Coordinador
    //Calculo de lineas por proceso
    int lineasporProcesos = lineasArchivo/numeroProcesos;
    int diferenciaLineProce = lineasArchivo - lineasporProcesos*numeroProcesos;
    if(diferenciaLineProce == 0 ){//Las lineas se distribuyen equitativamente en cantidad divLineProce
		int lineaInicia = 0;
        for (int  i = 0; i < numeroProcesos; i++){
            //crear proceso hijo y dar (lineasporProceso) Lineas
			instrucciones nuevaInstrucciones;
            nuevaInstrucciones.archivoEntrada = archivoEntrada;
            nuevaInstrucciones.lineaInicia= lineaInicia;
            nuevaInstrucciones.cadenaBuscar= cadenaBuscar;
            nuevaInstrucciones.lineasporProceso=lineasporProceso;
			pid = fork();
			
			if (pid == 0){
				//Soy el hijo
                close(pipes[ESCRITURA]);
                dup2(pipes[LECTURA],STDIN_FILENO);
				execv(ejecutableExec[0],ejecutableExec);
                exit(-1);


			}else if(pid > 0){
				//Soy tu padre!
                close(pipes[LECTURA]);
                write(pipes[ESCRITURA],nuevaInstrucciones,sizeof(instrucciones));
                waitpid(pid, &status,0);
                lineaInicia+= lineasporProcesos;



			}else{
				//Problemas
				exit(-1);
			}
			

        }


		//wait procesos y juntar


        
    }else if(diferenciaLineProce < 0){//El ultimo proceso queda con menos lineas
        for (int i = 0; i < numeroProcesos -1; i++){
            //crear proceso hijo y dar (lineasporProceso) Lineas
        }
        //dar procesos restantes (diferenciaLineProce)
    }else if(diferenciaLineProce > 0){//El ultimo proceso queda con (diferenciaLineProce) mas lineas.
        for (int i = 0; i < numeroProcesos -1; i++){
            //crear proceso hijo y dar (lineasporProceso) Lineas
        }
        //crear proceso hijo y dar procesos restantes (diferenciaLineProce) Lineas
    }


}