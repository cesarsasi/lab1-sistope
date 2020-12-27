char *mflag;
char* archivoVisibilidades,*archivoSalida;
int c;
int cantDiscos = 0;
int buffer = 0;
int anchoDiscos = 0;
while (( (c = getopt(argc, argv, "i:o:n:s:b")) != -1)){
    switch (c)
    {
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
    case 'n':
    mflag = optarg; 
    if(optarg == 0){
        printf("\nIngreso de parametro n (cantidad de discos) incorrecto!\n");
        exit(EXIT_FAILURE);
    }else{
        cantDiscos = atoi(optarg);
        if(cantDiscos >= 0){
        break;
        }
        else{
        printf("\nIngreso de parametro n (cantidad de discos) incorrecto!, debe ser mayor que cero\n"); //Verificacion pendiente
        exit(EXIT_FAILURE);
        }
    }
    case 'd':
    mflag = optarg; 
    if(optarg == 0){
        printf("\nIngreso de parametro d (ancho de discos) incorrecto!\n");
        exit(EXIT_FAILURE);
    }else{
        anchoDiscos = atoi(optarg);
        if(anchoDiscos >= 0){
        break;
        }
        else{
        printf("\nIngreso de parametro d (ancho de discos) incorrecto!, debe ser mayor que cero\n"); //Verificacion pendiente
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
        }
        else{
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

void escrituraResultados(char * archivoFinal, int idDisco, int mediaReal, int mediaImag, int potencia, int ruidoTotal){
    FILE* fileFinal = fopen(archivoFinal, "a");
    fprintf(fileFinal,"Disco: %d",idDisco);
    fprintf(fileFinal,"Media real: %d",mediaReal);
    fprintf(fileFinal,"Media imaginaria: %d",mediaImag);
    fprintf(fileFinal,"Potencia: %d",potencia);
    fprintf(fileFinal,"Ruido total %d:",ruidoTotal);
}