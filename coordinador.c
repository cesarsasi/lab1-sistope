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
    int status,accesoLectura,accesoLectura2;

    char *pl[] = {"comparador", NULL};

	//Programa coordinador                            Procesos no puede ser 0 !!!!!!!
    //Proceso Coordinador
    //Calculo de lineas por proceso
    int lineasporProcesos = lineasArchivo/numeroProcesos;                        
    int diferenciaLineProce = lineasArchivo - lineasporProcesos*numeroProcesos; 
    //if(diferenciaLineProce == 0 ){//Las lineas se distribuyen equitativamente en cantidad divLineProce
    //Condicion casos bordes 
    if(lineasporProcesos == 0){
        if(lineasArchivo<numeroProcesos){
            //añadir crear bandera para crear [lineasArchivo] archivos con 1 linea y el resto sin archivos
            lineasporProcesos = 1;
            accesoLectura = lineasArchivo;
            accesoLectura2 = lineasArchivo;
        }else if (lineasArchivo == 0){
            //crear procesos hijos sin lectura
            //Print error linea archivo CEROOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
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
            char nlineaInicia[100], nlineasporProcesos[100];
            sprintf(nlineaInicia,"%d",lineaInicia);
            sprintf(nlineasporProcesos,"%d",lineasporProcesos);

        printf("\n%d--------------------------------lpp\n",lineasporProcesos);
        printf("\n%d--------------------------------lIn\n",lineaInicia);
        //crear proceso hijo y dar (lineasporProceso) Lineas
        char nlineaInicia[100], nlineasporProcesos[100];
        sprintf(nlineaInicia,"%d",lineaInicia);
        sprintf(nlineasporProcesos,"%d",lineasporProcesos);

            pid = fork();
			if (pid == 0){
                //Soy el hijo
                close(arrPipes[i][ESCRITURA]); //Como el hijo no va a escribir, cierra el descriptor de escritura
                dup2(arrPipes[i][LECTURA], STDIN_FILENO);
                //nombre archivo parcial
                char nombreArchivoParcial[100]= "rp_";
	            strcat(nombreArchivoParcial,cadenaBuscar);
	            strcat(nombreArchivoParcial,"_");
	            int pidParcial = getpid();
	            char pidHijo[100];
	            sprintf(pidHijo,"%d",pidParcial);
	            strcat(nombreArchivoParcial,pidHijo);
	            strcat(nombreArchivoParcial,".txt");
	            
	            archivoRp=fopen("nombresRp.txt","a");
	            fprintf(archivoRp, "%s,", nombreArchivoParcial);
	            fclose(archivoRp);

            printf("mi padre debería ser el que tiene pid: %i\n", getppid() );
            dup2(arrPipes[i][LECTURA], STDIN_FILENO);
            //nombre archivo parcial
            char nombreArchivoParcial[100]= "rp_";
            strcat(nombreArchivoParcial,cadenaBuscar);
            strcat(nombreArchivoParcial,"_");
            int pidParcial = getpid();
            char pidHijo[100];
            sprintf(pidHijo,"%d",pidParcial);
            strcat(nombreArchivoParcial,pidHijo);
            strcat(nombreArchivoParcial,".txt");
            
            archivoRp=fopen("nombresRp.txt","a");
            fprintf(archivoRp, "%s,", nombreArchivoParcial);
            fclose(archivoRp);
            //excev

            if(lineasporProcesos == 0 && lineasArchivo>0 && accesoLectura>0){
                execv(pl[0], pl); 
                accesoLectura-=1;
            }else if(lineasporProcesos != 0){
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
			lineaInicia+= lineasporProcesos+2;
        }
        lineaInicia+= lineasporProcesos+2;

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
        
    }/*}else if(diferenciaLineProce < 0){//El ultimo proceso queda con menos lineas
        for (int i = 0; i < numeroProcesos -1; i++){
            //crear proceso hijo y dar (lineasporProceso) Lineas
        }
        fclose(archivoRp);
        fclose(archivoFinal);
        nombre = strtok(NULL,",");
    }
}