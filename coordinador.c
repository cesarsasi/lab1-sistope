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
    int **arrPipes = (int**)malloc(sizeof(int *)*numeroProcesos);
    for (int i = 0; i < numeroProcesos; i++){
        arrPipes[i]= (int*)malloc(sizeof(int)*2);
        pipe(arrPipes[i]);
    }
    
	pid_t pid;
    int status,controlAcceso1,controlAcceso2;

    char *pl[] = {"comparador", NULL};

	//Programa coordinador                            Procesos no puede ser 0 !!!!!!!
    //Proceso Coordinador
    //Calculo de lineas por proceso
    if(numeroProcesos==0){
    	if (flag == 1){
            printf("No puede haber 0 procesos!\n");
        }
    	exit(-1);
    }
    int lineasporProcesos = lineasArchivo/numeroProcesos;
    int diferenciaLineProce = lineasArchivo - lineasporProcesos*numeroProcesos;
    if(lineasporProcesos == 0){
        //caso no caen procesos en lineas
        if (lineasArchivo>0){
            lineasporProcesos = 1;
            controlAcceso1 = lineasArchivo;
            controlAcceso2 = lineasArchivo;
        }else{
            if (flag == 1){
                printf("No hay lineas que procesar!\n");
            }
            exit(-1);
        }
    }

    int lineaInicia = 0;
    //Creacion archivo final
    char nombreArchivoFinal[100]= "rc_";
    strcat(nombreArchivoFinal,cadenaBuscar);
    strcat(nombreArchivoFinal,".txt");
    FILE * archivoFinal;
    archivoFinal=fopen(nombreArchivoFinal,"w");
    fclose(archivoFinal);

    FILE *archivoRP;
    archivoRP=fopen("nombresRp.txt", "w");
    fclose(archivoRP);

    for (int  i = 0; i < numeroProcesos; i++){
        //crear proceso hijo y dar (lineasporProceso) Lineas
        if (diferenciaLineProce > 0 && i == numeroProcesos-1){
                lineasporProcesos += diferenciaLineProce;
            }
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
            dup2(arrPipes[i][LECTURA], STDIN_FILENO);
            //nombre archivo parcial
            
            char nombreArchivoParcial[100]= "archivos/rp_";
            strcat(nombreArchivoParcial,cadenaBuscar);
            strcat(nombreArchivoParcial,"_");
            int pidParcial = getpid();
            char pidHijo[100];
            sprintf(pidHijo,"%d",pidParcial);
            strcat(nombreArchivoParcial,pidHijo);
            strcat(nombreArchivoParcial,".txt");
            if(lineasporProcesos == 0 && controlAcceso1>0){
                archivoRp=fopen("nombresRp.txt","a");
                fprintf(archivoRp, "%s,", nombreArchivoParcial);
                fclose(archivoRp);
                //excev
                execv(pl[0], pl);
                controlAcceso1--;
            }else if(lineasporProcesos != 0){
                archivoRp=fopen("nombresRp.txt","a");
                fprintf(archivoRp, "%s,", nombreArchivoParcial);
                fclose(archivoRp);
                execv(pl[0], pl);
            }
        }else if(pid > 0){
            //Soy tu padre!
            close(arrPipes[i][LECTURA]); //El padre no va a leer, por lo tanto se cierra su descriptor
            write(arrPipes[i][ESCRITURA], instrucciones, 60*sizeof(char));
            waitpid(pid, &status,0);
        }else{
            //Problemas
            exit(-1);
        }
        lineaInicia+= lineasporProcesos;
        
    }

    //wait procesos y juntar formato rp_cadena_PID.txt y cantidad de procesos
    char lineasRp[200];
    archivoRp=fopen("nombresRp.txt","r");
    fgets(lineasRp,200,archivoRp);
    fclose(archivoRp);
    char lineasMostrar[200];

    if (flag == 1){
        printf("\nSE INGRESO BANDERA -d, LOS RESULTADOS SON:\n");
    }

    char *nombre = strtok(lineasRp,",");
    if (lineasporProcesos == 0){
        numeroProcesos = controlAcceso2;
    }
    
    for (int i = 0; i < numeroProcesos; i++){

        archivoFinal=fopen(nombreArchivoFinal,"a");
        archivoRp=fopen(nombre,"r");

        char data1;
        while((data1 = fgetc(archivoRp)) != EOF){
            fputc(data1, archivoFinal);
        }
        fclose(archivoRp);
        fclose(archivoFinal);

        if (flag == 1){
            archivoRp=fopen(nombre,"r");
            printf("\n%s\n",nombre);
            while((data1 = fgetc(archivoRp)) != EOF){
                printf("%c",data1);
            }
            fclose(archivoRp);
        }
        nombre = strtok(NULL,",");
    }
    if (flag == 1){
        char data2;
        printf("\n%s\n",nombreArchivoFinal);
        archivoFinal=fopen(nombreArchivoFinal,"r");
        while((data2 = fgetc(archivoFinal)) != EOF){
            printf("%c",data2);
        }
        fclose(archivoFinal);
    }
}