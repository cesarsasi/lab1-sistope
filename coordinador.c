//Includes
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h> //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t
#include <sys/stat.h>

#define ESCRITURA 1
#define LECTURA 0

FILE * archivoFinal;
FILE * archivoRp ;

//Funciones
//Main programa coordinador
int main(int argc, char** argv){
	

	int numeroProcesos,lineasArchivo;
    char* archivoEntrada,*cadenaBuscar;
    int flag;
    char *mflag;
    int c;
    
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

    int **arrPipes = (int**)malloc(sizeof(int *)*numeroProcesos);
    for (int i = 0; i < numeroProcesos; i++){
        arrPipes[i]= (int*)malloc(sizeof(int)*2);
        pipe(arrPipes[i]);
    }
    
	pid_t pid;
    int status;

    char *pl[] = {"comparador", NULL};

	//Programa coordinador                            Procesos no puede ser 0 !!!!!!!
    //Proceso Coordinador
    //Calculo de lineas por proceso
    int lineasporProcesos = lineasArchivo/numeroProcesos;
    int diferenciaLineProce = lineasArchivo - lineasporProcesos*numeroProcesos;
    if(diferenciaLineProce == 0 ){//Las lineas se distribuyen equitativamente en cantidad divLineProce
		int lineaInicia = 0;
        int *pidHijos = (int*)malloc(sizeof(int)*numeroProcesos);
        for (int  i = 0; i < numeroProcesos; i++){
            printf("\n%d--------------------------------lpp\n",lineasporProcesos);
            printf("\n%d--------------------------------lIn\n",lineaInicia);
            //crear proceso hijo y dar (lineasporProceso) Lineas
            char nlineaInicia[100], nlineasporProcesos[100];
            sprintf(nlineaInicia,"%d",lineaInicia);
            sprintf(nlineasporProcesos,"%d",lineasporProcesos);

			char instrucciones[60]={""};
            strcat(instrucciones,archivoEntrada);
            strcat(instrucciones,",");
            strcat(instrucciones,nlineaInicia);
            strcat(instrucciones,",");
            strcat(instrucciones,cadenaBuscar);
            strcat(instrucciones,",");
            strcat(instrucciones,nlineasporProcesos);
            strcat(instrucciones,",");

            pid = fork();
			if (pid == 0){
                //Soy el hijo
                close(arrPipes[i][ESCRITURA]); //Como el hijo no va a escribir, cierra el descriptor de escritura
                printf("al parecer soy el hijo y mi pid es: %i\n" , getpid());
                pidHijos[i]=getpid();
                printf("mi padre debería ser el que tiene pid: %i\n", getppid() );
                dup2(arrPipes[i][LECTURA], STDIN_FILENO);
                execv(pl[0], pl);

			}else if(pid > 0){
				//Soy tu padre!
                close(arrPipes[i][LECTURA]); //El padre no va a leer, por lo tanto se cierra su descriptor
                write(arrPipes[i][ESCRITURA], instrucciones, 60*sizeof(char));
                printf("al parecer soy el padre y mi pid es: %i\n" , getpid());
                waitpid(pid, &status,0);
                
			}else{
				//Problemas
				exit(-1);
			}
			lineaInicia+= lineasporProcesos+2;
        }

		//wait procesos y juntar formato rp_cadena_PID.txt y cantidad de procesos
        char nombreArchivoFinal[60]= {""};
        strcat(nombreArchivoFinal,"rc_");
        strcat(nombreArchivoFinal,cadenaBuscar);
        strcat(nombreArchivoFinal,".txt");
        printf("\nHOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOLA\n");
        archivoFinal = fopen(nombreArchivoFinal, "w");
        fclose (archivoFinal);

        printf("\n//////////////////////// %s",nombreArchivoFinal);
        for (int i = 0; i < numeroProcesos; i++){
            //printf("\n//////////////////////// %s",archivoFinal);
            
            char nombreArchivoParcial[60]= {""};
            strcat(nombreArchivoParcial,"rp_");
            strcat(nombreArchivoParcial,cadenaBuscar);
            strcat(nombreArchivoParcial,"_");
            char pidHijo[100];
            sprintf(pidHijo,"%d",pidHijos[i]);
            strcat(nombreArchivoParcial,pidHijo);
            strcat(nombreArchivoParcial,".txt");
            char data1;
            archivoRp = fopen(nombreArchivoParcial, "r");
            archivoFinal = fopen (nombreArchivoFinal,"a");
            
            while((data1 = fgetc(archivoRp)) != EOF){
                fputc(data1, archivoFinal);
            }
            fclose(archivoRp);
            fclose(archivoFinal);
        }
        



        
    }/*}else if(diferenciaLineProce < 0){//El ultimo proceso queda con menos lineas
        for (int i = 0; i < numeroProcesos -1; i++){
            //crear proceso hijo y dar (lineasporProceso) Lineas
        }
        //dar procesos restantes (diferenciaLineProce)
    }else if(diferenciaLineProce > 0){//El ultimo proceso queda con (diferenciaLineProce) mas lineas.
        for (int i = 0; i < numeroProcesos -1; i++){
            //crear proceso hijo y dar (lineasporProceso) Lineas
        }
        //crear proceso hijo y dar procesos restantes (diferenciaLineProce) Lineas
    }*/


}