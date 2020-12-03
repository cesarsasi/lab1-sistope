/*
Laboratorio 2
Profesor   : Fernando Rannou
Ayudante   : Benjamin Muñoz
Integrantes:    
-                Octavio Nicolas Canales Ñirriman 20.003.610-7
-                César Antonio Salazar Silva      19.916.471-6
Sistemas Operativos 2.2020  (Creación: 2 de Diciembre del 2020)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>

int heightOriginal = 0;
int widthOriginal = 0;
int bins = 0;
int rangoBins = 0;
int ultimoNivel= 0;
int flag = 0;
/*
Estructura : Estructura para almacenar primera cabezera de la imagen
Observaciones: Es necesaria para implementar la lectura de la imagen 
*/
typedef struct bmpFileHeader{ /* 2 bytes de identificación */
  unsigned int size;        /* Tamaño del archivo */
  uint16_t resv1;           /* Reservado */
  uint16_t resv2;           /* Reservado */
  unsigned int offset;      /* Offset hasta hasta los datos de imagen */
} bmpFileHeader;
/*
Estructura : Estructura para almacenar la segunda cabezera de la imagen
Observaciones: Guarda informacion como tamaño, bits, etc. Es de suma importancia para guardar la imagen
*/
typedef struct bmpInfoHeader{
  unsigned int headersize;  /* Tamaño de la cabecera       */
  int width;                /* Ancho                       */
  int height;               /* Alto                        */
  uint16_t planes;          /* Planos de color (Siempre 1) */
  uint16_t bpp;             /* bits por pixel */
  unsigned int compress;    /* compresión */
  unsigned int imgsize;     /* tamaño de los datos de imagen */
  int bpmx;                 /* Resolución X en bits por metro */
  int bpmy;                 /* Resolución Y en bits por metro */
  unsigned int colors;      /* colors used en la paleta */
  unsigned int imxtcolors;  /* Colores importantes. 0 si son todos */
} bmpInfoHeader;
/*
Estructura : Estructura que almacena la imagen
Observaciones: Guarda b, g, r debido a la forma en que estan almacenados los pixles en un aimgen .bmp
*/
typedef struct IMAGErgb{                                                                                                                                                                                                                             
    unsigned char  b;                                                                                                                                                                                                                        
    unsigned char  g;                                                                                                                                                                                                                        
    unsigned char  r; 
    unsigned char alfa;

} IMAGErgb;
/*
Estructura : Estructura que permite el almacenamiento de un cuadrante
Observaciones: Se guarda el cuadrante, lvl , altura, anchura, y cuadrantes 1, 2, 3, 4
*/
typedef struct cuadrante{  
    IMAGErgb ** cuadrantePrincipal;
    int * histograma;  
    int lvl; 
    int height;
    int width;                                                                                                                                                                                                                       
    struct cuadrante * subCuadrante1;                                                                                                                                                                                                                        
    struct cuadrante * subCuadrante2;                                                                                                                                                                                                                        
    struct cuadrante * subCuadrante3;
    struct cuadrante * subCuadrante4;                                                                                                                                                                                                                        
} cuadrante;
//Definicion previa de las funciones implementadas
void *crearHebras(void *padre);
IMAGErgb **LoadBMP(char *filename, bmpInfoHeader *bInfoHeader);
void DisplayInfo(bmpInfoHeader *info);
void asignarImgSubCuadrantes(cuadrante * structPadre);
void calcularHistograma(cuadrante * structPadre);
void sumarHistogramas(cuadrante * structPadre);


//
/*
FUNCION : Funcion main
Entrada : Parametros ingresados por usuario al momento de ejecutar el programa, tales como (Ver observaciones).
Salida  : Retorno a 0 al terminar programa
Observaciones: 
1) ./lab2 -i imagen_1.bmp -o histograma.txt -L 4 -B 4 
2) ./lab2 -i imagen_1.bmp -o histograma.txt -L 4 -B 4 -d
*/
int main(int argc, char** argv){
  
  int niveles,numerodeBins;
  char* archivoImagen,*archivoHistograma;
  char *mflag;
  int c;
  while (( (c = getopt(argc, argv, "i:o:L:B:d")) != -1)){
    switch (c)
    {
    case 'i':
      mflag = optarg; 
      if(optarg == 0){
        printf("\nIngreso de parametro i incorrecto! \n");
        exit(EXIT_FAILURE);
      }else{
        archivoImagen = optarg;
        FILE* fileFinal = fopen(archivoImagen, "r");
        if(fileFinal==NULL){
          printf("\nIngreso de parametro i incorrecto!, el archivo no existe \n");
          exit(EXIT_FAILURE);
        }
        fclose(fileFinal);
      }
      break;
    case 'o':
      mflag = optarg; 
      if(optarg == 0){
        printf("\nIngreso de parametro o incorrecto!\n");
        exit(EXIT_FAILURE);
      }else{
        archivoHistograma = optarg;
      }
      break;
    case 'L':
      mflag = optarg; 
      if(optarg == 0){
        printf("\nIngreso de parametro L incorrecto!\n");
        exit(EXIT_FAILURE);
      }else{
        niveles = atoi(optarg);
        if(fmod(log2((double)niveles),1.0)==0.0 && niveles>0 && niveles<9){
          break;
        }
        else{
          printf("\nIngreso de parametro L incorrecto!, debe ser digito y potencia de 2\n");
          exit(EXIT_FAILURE);
        }
        
      }
      break;      
    case 'B':
      if(optarg == 0){
        printf("\nIngreso de parametro B incorrecto!\n");
        exit(EXIT_FAILURE);
      }else{
        numerodeBins = atoi(optarg);
        if(fmod(log2((double)numerodeBins),1.0)==0.0 && numerodeBins>1){
          break;
        }
        else{
          printf("\nIngreso de parametro B incorrecto!, debe ser digito y potencia de 2\n");
          exit(EXIT_FAILURE);
        }
      }
      break;
    case 'd':
      flag = 1;
      break;
    }
  }
  //Dar valor a variables globales
  ultimoNivel = niveles;       // Nivel maximo
  bins = numerodeBins;         // Cantidad de intervalos
  rangoBins = 256/numerodeBins;// Rango
  //Instanciar header de la imagen y puntero en el que se va a almacenar
  bmpInfoHeader info;
  IMAGErgb **img;
  //Asignamos un puntero a la matriz de la imagen obtenida
  img=LoadBMP(archivoImagen, &info);
  //Printear info de la imagen
  if (flag == 1){
    DisplayInfo(&info);
  }
  //Creamos Struct nivel 0
  cuadrante * cuadrantePadre = (cuadrante*)calloc(sizeof(cuadrante),1);
  int *histograma = (int*)calloc(sizeof(int),bins);
  cuadrantePadre->cuadrantePrincipal = img;
  cuadrantePadre->height=heightOriginal;
  cuadrantePadre->width=widthOriginal;
  cuadrantePadre->lvl= 0;
  cuadrantePadre->histograma = histograma;
  //calcularHistograma(cuadrantePadre);
  pthread_t hebra;
  pthread_create(&hebra, NULL, crearHebras, (void*)cuadrantePadre);
  pthread_join(hebra,NULL);
  //Mostrar e imprimir resultado
  if (flag == 1){
    printf("\n--------------- HISTOGRAMA PADRE ---------------\n");
  }
  int limiteInf = 0;
  int limiteSup = limiteInf + rangoBins-1;
  FILE* fileFinal = fopen(archivoHistograma, "w");
  for (int i = 0; i < bins; ++i){
    fprintf(fileFinal,"[%d,    %d]    %d",limiteInf, limiteSup, cuadrantePadre->histograma[i]);
    limiteInf+=rangoBins;
    limiteSup+=rangoBins;
    fprintf(fileFinal, "\n");
    if (flag == 1){
      printf("[%d,    %d]    %d",limiteInf, limiteSup, cuadrantePadre->histograma[i]);
      printf("\n");
    }
  }
  return 0;
}

/*
FUNCION : Crear hebras de subcuadrantes y esperar el termino de su ejecucion (recursivo)
ademas implementa el calculo de histograma para cada cuadrante. 
Entrada : Como parametro de entrada recibe un tipo void*, sin embargo, este es renombrado dentro de la funcion
a una estructura del tipo cuadrante.
Salida  : void
*/
void *crearHebras(void *padre){
  cuadrante * padrecito = (cuadrante *)padre;
  asignarImgSubCuadrantes(padrecito);
  pthread_t hebras[4];
  if(padrecito->lvl < ultimoNivel){
    pthread_create(&hebras[0],NULL, crearHebras,(void*)padrecito->subCuadrante1);
    pthread_join(hebras[0],NULL);
    pthread_create(&hebras[1],NULL, crearHebras,(void*)padrecito->subCuadrante2);
    pthread_join(hebras[1],NULL);
    pthread_create(&hebras[2],NULL, crearHebras,(void*)padrecito->subCuadrante3);
    pthread_join(hebras[2],NULL);
    pthread_create(&hebras[3],NULL, crearHebras,(void*)padrecito->subCuadrante4);
    pthread_join(hebras[3],NULL);
  }else{
    calcularHistograma(padrecito);
    pthread_exit(NULL);
  }
  sumarHistogramas(padrecito);
  pthread_exit(NULL);
}

/*
FUNCION : Guarda la imagen en el puntero asociado
Entrada : Recibe el puntero al nombre del archivo y un puntero a un tipo estructura bmInfoHeader
Salida  : Estructura tipo IMAGErgb
Observaciones: Los bits por pixel son un elemento a tener en cuenta, ya que, con 32 se tiene una cuarta componente un la imagen bmp
*/
IMAGErgb **LoadBMP(char *filename, bmpInfoHeader *bInfoHeader){
  FILE *f;
  bmpFileHeader header;       /* cabecera */
  //unsigned char *imgdata;   /* datos de imagen */
  uint16_t type;              /* 2 bytes identificativos */
  f=fopen (filename, "rb");
  if (!f)
    return NULL;              /* Si no podemos leer, no hay imagen, leemos los dos primeros bytes */
  fread(&type, sizeof(uint16_t), 1, f);
  if (type !=0x4D42){         /* Comprobamos el formato */
      fclose(f);
      return NULL;
    }                         /* Leemos la cabecera de fichero completa */
  fread(&header, sizeof(bmpFileHeader), 1, f);
  /* Leemos la cabecera de información completa */
  fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);
  /* Nos situamos en el sitio donde empiezan los datos de imagen, nos lo indica el offset de la cabecera de fichero*/
  fseek(f, header.offset, SEEK_SET);
  /* Reservamos memoria para la imagen, ¿cuánta? Tanto como indique imgsize */
  IMAGErgb **imgdata=(IMAGErgb**)calloc(sizeof(IMAGErgb*),bInfoHeader->height);
  heightOriginal=bInfoHeader->height;
  widthOriginal=bInfoHeader->width;
  for(int i=0; i< bInfoHeader->height;i++){
    imgdata[i]= (IMAGErgb*)calloc(sizeof(IMAGErgb),bInfoHeader->width);    
  }
  for(size_t i=0;i<bInfoHeader->height;i++){
    for(size_t j=0; j<bInfoHeader->width;j++){
      fread(&imgdata[i][j],sizeof(unsigned char),sizeof(IMAGErgb),f);
    }
  }
  /* Cerramos el archivo */
  fclose(f);
  /* Devolvemos la imagen */
  return imgdata;
} 

/*
FUNCION : Printea los parametros contenidos en la segunda cabezera de la imagen bmp
Entrada : puntero a estructura del tipo bmpInfoHeader
Salida  : void
Observaciones: Se utiliza solo para motivos informativos acerca de la imagen
*/
void DisplayInfo(bmpInfoHeader *info){
  printf("Tamaño de la cabecera: %u\n", info->headersize);
  printf("Anchura: %d\n", info->width);
  printf("Altura: %d\n", info->height);
  printf("Planos (1): %d\n", info->planes);
  printf("Bits por pixel: %d\n", info->bpp);
  printf("Compresión: %d\n", info->compress);
  printf("Tamaño de datos de imagen: %u\n", info->imgsize);
  printf("Resolucón horizontal: %u\n", info->bpmx);
  printf("Resolucón vertical: %u\n", info->bpmy);
  printf("Colores en paleta: %d\n", info->colors);
  printf("Colores importantes: %d\n", info->imxtcolors);
}


/*
FUNCION : Asignar cuadrante a hijo, rellena cada subcuadrante respectivamente con la informacion correspondiente
Entrada : Puntero a una estructura del tipo cuadrante
Salida  : void
Observaciones: Detalle grafico que permite analizar la particion realizada por cuadrante 
0     w/2      w
subc1    subc2
0 0 0 0| 0 0 0 0 0      primer cuadrante : h = 0   -> h/2
0 0 0 0| 0 0 0 0                           w = 0   -> w/2
0 0 0 0| 0 0 0 0        Segundo cuadrante: h = 0   -> h/2
0 0 0 0| 0 0 0 0                           w = w/2 -> w
---------------- h/2    Tercer cuadrante : h = h/2 -> h
0 0 0 0| 0 0 0 0                           w = 0   -> w/2    
0 0 0 0| 0 0 0 0        Cuarto cuadrante : h = h/2 -> h
0 0 0 0| 0 0 0 0                           w = w/2 -> w
0 0 0 0| 0 0 0 0 h
subc3    subc4                                            
*/
void asignarImgSubCuadrantes(cuadrante * structPadre){  
  int height= structPadre->height;
  int width = structPadre->width;
  int lvlPadre = structPadre->lvl;
  //Asignamos memoria a el subcuadrante
  IMAGErgb **imgsubcuadrante1=(IMAGErgb**)calloc(sizeof(IMAGErgb*),(height/2));
  for(int i=0; i< (height/2);i++){
    imgsubcuadrante1[i]= (IMAGErgb*)calloc(sizeof(IMAGErgb),(width/2));    
  }
  //Poblamos el subcuadrante
  int h1 = 0, w1 = 0;
  for(int i = 0; i<height/2; i++ ){
    for(int j = 0; j<width/2; j++){
      imgsubcuadrante1[i][j].r = structPadre->cuadrantePrincipal[i][j].r;
      imgsubcuadrante1[i][j].g = structPadre->cuadrantePrincipal[i][j].g;
      imgsubcuadrante1[i][j].b = structPadre->cuadrantePrincipal[i][j].b;
    }
  }
  //Apuntar esta matriz al subcuandrante del padre correspondiente
  cuadrante * subcuadrante = (cuadrante*)calloc(sizeof(cuadrante),1);
  subcuadrante->cuadrantePrincipal = imgsubcuadrante1;
  int *histograma1 = (int*)calloc(sizeof(int),bins);
  subcuadrante->histograma = histograma1;
  subcuadrante->height = height/2;
  subcuadrante->width = width/2;
  subcuadrante->lvl = lvlPadre +1;
  subcuadrante->subCuadrante1 = NULL;
  subcuadrante->subCuadrante2 = NULL;
  subcuadrante->subCuadrante3 = NULL;
  subcuadrante->subCuadrante4 = NULL;
  structPadre->subCuadrante1 = subcuadrante;
  //Asignamos memoria a el subcuadrante
  IMAGErgb **imgsubcuadrante2=(IMAGErgb**)calloc(sizeof(IMAGErgb*),(height/2));
  for(int i=0; i< (height/2);i++){
    imgsubcuadrante2[i]= (IMAGErgb*)calloc(sizeof(IMAGErgb),(width/2));    
  }
  int h2 = 0, w2 = 0;
  for(int i = 0; i<height/2; i++ ){
    for(int j = width/2; j<width; j++){
      imgsubcuadrante2[h2][w2].r = structPadre->cuadrantePrincipal[i][j].r;
      imgsubcuadrante2[h2][w2].g = structPadre->cuadrantePrincipal[i][j].g;
      imgsubcuadrante2[h2][w2].b = structPadre->cuadrantePrincipal[i][j].b;
      w2++;
    }
    w2=0;
    h2++;
  }
  //Apuntar esta matriz al subcuandrante del padre correspondiente
  cuadrante * subcuadrante2 = (cuadrante*)calloc(sizeof(cuadrante),1);
  subcuadrante2->cuadrantePrincipal = imgsubcuadrante2;
  int *histograma2 = (int*)calloc(sizeof(int),bins);
  subcuadrante2->histograma = histograma2;
  subcuadrante2->height = height/2;
  subcuadrante2->width = width/2;
  subcuadrante2->lvl = lvlPadre +1;
  subcuadrante2->subCuadrante1 = NULL;
  subcuadrante2->subCuadrante2 = NULL;
  subcuadrante2->subCuadrante3 = NULL;
  subcuadrante2->subCuadrante4 = NULL;
  structPadre->subCuadrante2 = subcuadrante2;

  //Asignamos memoria a el subcuadrante
  IMAGErgb **imgsubcuadrante3=(IMAGErgb**)calloc(sizeof(IMAGErgb*),(height/2));
  for(int i=0; i< (height/2);i++){
    imgsubcuadrante3[i]= (IMAGErgb*)calloc(sizeof(IMAGErgb),(width/2));    
  }

  int h3 = 0, w3 = 0;
  for(int i = height/2; i<height; i++ ){
    for(int j = 0; j<width/2; j++){
      imgsubcuadrante3[h3][w3].r = structPadre->cuadrantePrincipal[i][j].r;
      imgsubcuadrante3[h3][w3].g = structPadre->cuadrantePrincipal[i][j].g;
      imgsubcuadrante3[h3][w3].b = structPadre->cuadrantePrincipal[i][j].b;
      w3++;
    }
    w3=0;
    h3++;
  }
  //Apuntar esta matriz al subcuandrante del padre correspondiente
  cuadrante * subcuadrante3 = (cuadrante*)calloc(sizeof(cuadrante),1);
  subcuadrante3->cuadrantePrincipal = imgsubcuadrante3;
  int *histograma3 = (int*)calloc(sizeof(int),bins);
  subcuadrante3->histograma = histograma3;
  subcuadrante3->height = height/2;
  subcuadrante3->width = width/2;
  subcuadrante3->lvl = lvlPadre +1;
  subcuadrante3->subCuadrante1 = NULL;
  subcuadrante3->subCuadrante2 = NULL;
  subcuadrante3->subCuadrante3 = NULL;
  subcuadrante3->subCuadrante4 = NULL;
  structPadre->subCuadrante3 = subcuadrante3;

  //Asignamos memoria a el subcuadrante
  IMAGErgb **imgsubcuadrante4=(IMAGErgb**)calloc(sizeof(IMAGErgb*),(height/2));
  for(int i=0; i< (height/2);i++){
    imgsubcuadrante4[i]= (IMAGErgb*)calloc(sizeof(IMAGErgb),(width/2));    
  }
  int h4 = 0, w4 = 0;
  for(int i = height/2; i<height; i++ ){
    for(int j = width/2; j<width; j++){
      imgsubcuadrante4[h4][w4].r = structPadre->cuadrantePrincipal[i][j].r;
      imgsubcuadrante4[h4][w4].g = structPadre->cuadrantePrincipal[i][j].g;
      imgsubcuadrante4[h4][w4].b = structPadre->cuadrantePrincipal[i][j].b;
      w4++;
    }
    w4=0;
    h4++;
  }
  //Apuntar esta matriz al subcuandrante del padre correspondiente
  cuadrante * subcuadrante4 = (cuadrante*)calloc(sizeof(cuadrante),1);
  subcuadrante4->cuadrantePrincipal = imgsubcuadrante4;
  int *histograma4 = (int*)calloc(sizeof(int),bins);
  subcuadrante4->histograma = histograma4;
  subcuadrante4->height = height/2;
  subcuadrante4->width = width/2;
  subcuadrante4->lvl = lvlPadre +1;
  subcuadrante4->subCuadrante1 = NULL;
  subcuadrante4->subCuadrante2 = NULL;
  subcuadrante4->subCuadrante3 = NULL;
  subcuadrante4->subCuadrante4 = NULL;
  structPadre->subCuadrante4 = subcuadrante4;
};

/*
FUNCION : Calcular histograma de x struct Cuadrante
Entrada : Puntero a estructura del tipo cuadrante
Salida  : void  
Observaciones: Convierte a gris los pixeles de la imagen, calcula su valor en la escala de grises y lo asigna a un intervalo del histograma 
*/
void calcularHistograma(cuadrante *structPadre){
  //Proceso calcular histograma
  for(int i=0;i<structPadre->height;i++){
    for(int j=0; j<structPadre->width;j++){
      //Convertir rgb a gris
      int valorGris = structPadre->cuadrantePrincipal[i][j].r * 0.3 + structPadre->cuadrantePrincipal[i][j].g * 0.59 + structPadre->cuadrantePrincipal[i][j].b * 0.11;
      int limiteInf = 0;
      int limiteSup = limiteInf + rangoBins-1;
      //Sumar al contador de la seccion del histograma a la que corresponde
      int posicionHistograma = floor(valorGris/rangoBins);
      structPadre->histograma[posicionHistograma] +=1;
    }
  }
  //PrintearHistograma
  if (flag == 1){
    printf("\n--------------- HISTOGRAMA HEBRA ---------------\n");
    for (int i = 0; i < bins; ++i){
      //printf("\n[%d,  %d]",inicioRango,finRango);
      printf("[%d] ",structPadre->histograma[i]);
    }
  }
}

/*
FUNCION : Sumar histogramas hijos de un cuadrante y almacenarlo en el padre
Entrada : Puntero a estructura del tipo cuadrante
Salida  : void
Observaciones: Suma el resultado de los histogramas de los subcuadrantes por cada intervalo y los asigna a el cuadrante principal  
*/
void sumarHistogramas(cuadrante * structPadre){
  for (int i = 0; i < bins; ++i){
    structPadre->histograma[i] = structPadre->subCuadrante1->histograma[i] + structPadre->subCuadrante2->histograma[i] +structPadre->subCuadrante3->histograma[i] +structPadre->subCuadrante4->histograma[i];
  }
}