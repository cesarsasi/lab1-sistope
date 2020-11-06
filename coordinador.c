/*
Laboratorio 1
Profesor   : Rannou
Ayudante   : Benjamin
Integrantes:    Octavio Canales 
                César Antonio Salazar Silva  19.916.471-6
Sistemas Operativos 2.2020  (Creación: 01-Noviembre-2020)
*/

//Se extienden librerias
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>  //Define las constantes simbólicas para usar con waitpid(), wait() por ejemplo
#include <sys/types.h> //define varios tipos de datos como pid_t
#include <sys/stat.h>

#define ESCRITURA 1
#define LECTURA 0
//Variable Global para archivos con resultados parciales.
FILE * archivoRp ;


//BLOQUE PRINCIPAL - FUNCIONALIDAD COORDINADOR
//Entrada : Recibe como entrada todos los argumentos de la consola, Archivo a leer, Cantidad de procesos, Cantidad de lineas, Cadena a Buscar y flag
//Salida  : No retorna nada
int main(int argc, char** argv){
	int numeroProcesos,lineasArchivo;
    char* archivoEntrada,*cadenaBuscar;
    int flag;
    char *mflag;
    int c;
    //Leer contenido de los parametros iniciales en el comando ejecutado
    while (( (c = getopt(argc, argv, "i:n:c:p:d")) != -1)){
        switch (c)
        {
        case 'i':
			//
            mflag = optarg; 
			if(optarg == 0){
				printf("\nIngreso de parametro i incorrecto! \n");
                exit(EXIT_FAILURE);
			}else{
				archivoEntrada = optarg;
			}
        	break;
        case 'n':
			//
            mflag = optarg; 
            if(optarg == 0){
				printf("\nIngreso de parametro n incorrecto!\n");
                exit(EXIT_FAILURE);
			}else{
				numeroProcesos = atof(optarg);
			}
        	break;
		case 'c':
			//
			mflag = optarg; 
            if(optarg == 0){
				printf("\nIngreso de parametro c incorrecto!\n");
                exit(EXIT_FAILURE);
			}else{
				lineasArchivo = atof(optarg);
			}
        	break;			
        case 'p':
			//
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
    //Se inicializa un arreglo de pipes
    int **arrPipes = (int**)malloc(sizeof(int *)*numeroProcesos);
    for (int i = 0; i < numeroProcesos; i++){
        arrPipes[i]= (int*)malloc(sizeof(int)*2);
        pipe(arrPipes[i]);
    }
    //Se inicializa la variable pid_t para manejar los procesos
	pid_t pid;
    int status,controlAcceso1,controlAcceso2;
    int bandera = 0;
    //Se utiliza este arreglo para guardar el nombre de un programa ejecutable[Arreglo unidimensional de caracteres]
    char *pl[] = {"comparador", NULL};
	//De ser 0 procesos no se ejecuta el programa                           
    if(numeroProcesos==0){
    	if (flag == 1){
            printf("No puede haber 0 procesos!\n");
        }
    	exit(-1);
    }
    //Proceso coordinador
    //Se calcula cuantas lineas por proceso debe haber y cuantas lineas sobran desde el ultimo proceso
    int lineasporProcesos = lineasArchivo/numeroProcesos;
    int diferenciaLineProce = lineasArchivo - lineasporProcesos*numeroProcesos;
    //En el caso que la division sea cero por aproximacion a entero
    if(lineasporProcesos == 0){
        //En caso de haber lineas por leer se modifican para leer una linea por proceso
        if (lineasArchivo>0){
            lineasporProcesos = 1;
            bandera=1;
            controlAcceso1 = lineasArchivo;
            controlAcceso2 = lineasArchivo;
        //En caso de no haber lineas no se ejecuta el programa
        }else{
            if (flag == 1){
                printf("No hay lineas que procesar!\n");
            }
            exit(-1);
        }
    }
    //Se inicializa variable en la que comenzara la lectura
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
        //En caso de ser la ultima iteracion ingresa a los casos bordes por mas cantidad de lineas que los otros procesos
        if (diferenciaLineProce > 0 && i == numeroProcesos-1){
            lineasporProcesos += diferenciaLineProce;
        }
        //En caso de ser Caso borde con lineas menor que procesos, se resta hasta tener 0 lineas
        if(bandera == 1 && controlAcceso1>=0){
            controlAcceso1-=1;
        }
        //Concadenamos las variables que se pasarán por el pipe
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
        //Se crea un proceso
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
            if(bandera == 1 && controlAcceso1>=0){
                archivoRp=fopen("nombresRp.txt","a");
                fprintf(archivoRp, "%s,", nombreArchivoParcial);
                fclose(archivoRp);
                //excev
                execv(pl[0], pl);
            }else if(bandera == 0){
                archivoRp=fopen("nombresRp.txt","a");
                fprintf(archivoRp, "%s,", nombreArchivoParcial);
                fclose(archivoRp);
                execv(pl[0], pl);
            }
            if(bandera == 1 && controlAcceso1<0){
                exit(-1);
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
    //Esperar procesos, asi juntar los archivos con formato rp_cadena_PID.txt
    //Almacenar los nombres de los resultados parciales creados, (son tantos procesos se hayan creado)
    char lineasRp[1024];
    archivoRp=fopen("nombresRp.txt","r");
    fgets(lineasRp,1024,archivoRp);
    fclose(archivoRp);
    //----------------------------------------------------------------------------------------------------------- En que se usa esta variable????!!!!
    char lineasMostrar[1024];
    //Aviso de detección de bandera
    if (flag == 1){
        printf("\nSE INGRESO BANDERA -d, LOS RESULTADOS SON:\n");
    }
    char *nombre = strtok(lineasRp,",");
    if (bandera == 1){
        numeroProcesos = controlAcceso2;
    }
    //Agregar contenido al archivo final, juntando todos los resultados parciales
    for (int i = 0; i < numeroProcesos; i++){
        archivoFinal=fopen(nombreArchivoFinal,"a");
        archivoRp=fopen(nombre,"r");
        char data1;
        while((data1 = fgetc(archivoRp)) != EOF){
            fputc(data1, archivoFinal);
        }
        fclose(archivoRp);
        fclose(archivoFinal);
        //Imprimir el archivo parcial si hay flag -d 
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
    //Imprimir el archivo final si hay flag -d
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