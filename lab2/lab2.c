#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

typedef struct bmpFileHeader
{
  /* 2 bytes de identificación */
  unsigned int size;        /* Tamaño del archivo */
  uint16_t resv1;       /* Reservado */
  uint16_t resv2;       /* Reservado */
  unsigned int offset;      /* Offset hasta hasta los datos de imagen */
} bmpFileHeader;

typedef struct bmpInfoHeader
{
  unsigned int headersize;      /* Tamaño de la cabecera */
  int width;       /* Ancho */
  int height;      /* Alto */
  uint16_t planes;          /* Planos de color (Siempre 1) */
  uint16_t bpp;             /* bits por pixel */
  unsigned int compress;        /* compresión */
  unsigned int imgsize;     /* tamaño de los datos de imagen */
  int bpmx;        /* Resolución X en bits por metro */
  int bpmy;        /* Resolución Y en bits por metro */
  unsigned int colors;      /* colors used en la paleta */
  unsigned int imxtcolors;      /* Colores importantes. 0 si son todos */
} bmpInfoHeader;


typedef struct IMAGErgb{                                                                                                                                                                                                                             
    unsigned char  r;                                                                                                                                                                                                                        
    unsigned char  g;                                                                                                                                                                                                                        
    unsigned char  b;                                                                                                                                                                                                                        
} IMAGErgb;

typedef struct cuadrante{  
    IMAGErgb ** cuadrantePrincipal;  
    int lvl; 
    int height;
    int width;                                                                                                                                                                                                                       
    struct cuadrante * subCuadrante1;                                                                                                                                                                                                                        
    struct cuadrante * subCuadrante2;                                                                                                                                                                                                                        
    struct cuadrante * subCuadrante3;
    struct cuadrante * subCuadrante4;                                                                                                                                                                                                                        
} cuadrante;
/*
[structPadre]                                                             lvl0 por hebra  el padre espere al hijo
[[hijo] [hijo] [hijo] [hijo] ]    lvl1
*/

void asignarImgSubCuadrantes(cuadrante * structPadre){  
  int height= structPadre->height;
  int width = structPadre->width;
  int lvlPadre = structPadre->lvl;
  //Matriz Primer subcuadrante
  IMAGErgb **imgsubcuadrante1=(IMAGErgb**)malloc(sizeof(IMAGErgb*)*(height/2));
  for(int i=0; i< (height/2);i++){
    imgsubcuadrante1[i]= (IMAGErgb*)malloc(sizeof(IMAGErgb)*(width/2));    
  }
  int h1 = 0, w1 = 0;
  for(int i = 0; i<height/2; i++ ){
    for(int j = 0; j<width/2; j++){
      imgsubcuadrante1[h1][w1].r = structPadre->cuadrantePrincipal[i][j].r;
      imgsubcuadrante1[h1][w1].g = structPadre->cuadrantePrincipal[i][j].g;
      imgsubcuadrante1[h1][w1].b = structPadre->cuadrantePrincipal[i][j].b;
      w1++;
    }
    w1=0;
    h1++;
  }
  //Apuntar esta matriz al subcuandrante del padre correspondiente
  cuadrante * subcuadrante = (cuadrante*)malloc(sizeof(cuadrante)*1);
  subcuadrante->cuadrantePrincipal = imgsubcuadrante1;
  subcuadrante->height = height/2;
  subcuadrante->width = width/2;
  subcuadrante->lvl = lvlPadre +1;
  subcuadrante->subCuadrante1 = NULL;
  subcuadrante->subCuadrante2 = NULL;
  subcuadrante->subCuadrante3 = NULL;
  subcuadrante->subCuadrante4 = NULL;
  structPadre->subCuadrante1 = subcuadrante;

  //Matriz segundo subcuadrante
  IMAGErgb **imgsubcuadrante2=(IMAGErgb**)malloc(sizeof(IMAGErgb*)*(height/2));
  for(int i=0; i< (height/2);i++){
    imgsubcuadrante2[i]= (IMAGErgb*)malloc(sizeof(IMAGErgb)*(width/2));    
  }
  int h2 = 0, w2 = width/2;
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
  cuadrante * subcuadrante2 = (cuadrante*)malloc(sizeof(cuadrante)*1);
  subcuadrante2->cuadrantePrincipal = imgsubcuadrante2;
  subcuadrante2->height = height/2;
  subcuadrante2->width = width/2;
  subcuadrante2->lvl = lvlPadre +1;
  subcuadrante2->subCuadrante1 = NULL;
  subcuadrante2->subCuadrante2 = NULL;
  subcuadrante2->subCuadrante3 = NULL;
  subcuadrante2->subCuadrante4 = NULL;
  structPadre->subCuadrante2 = subcuadrante2;

  //Matriz tercer subcuadrante
  IMAGErgb **imgsubcuadrante3=(IMAGErgb**)malloc(sizeof(IMAGErgb*)*(height/2));
  for(int i=0; i< (height/2);i++){
    imgsubcuadrante3[i]= (IMAGErgb*)malloc(sizeof(IMAGErgb)*(width/2));    
  }
  int h3 = height/2, w3 = 0;
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
  cuadrante * subcuadrante3 = (cuadrante*)malloc(sizeof(cuadrante)*1);
  subcuadrante3->cuadrantePrincipal = imgsubcuadrante3;
  subcuadrante3->height = height/2;
  subcuadrante3->width = width/2;
  subcuadrante3->lvl = lvlPadre +1;
  subcuadrante3->subCuadrante1 = NULL;
  subcuadrante3->subCuadrante2 = NULL;
  subcuadrante3->subCuadrante3 = NULL;
  subcuadrante3->subCuadrante4 = NULL;
  structPadre->subCuadrante3 = subcuadrante3;

  //Matriz cuarto subcuadrante
  IMAGErgb **imgsubcuadrant4=(IMAGErgb**)malloc(sizeof(IMAGErgb*)*(height/2));
  for(int i=0; i< (height/2);i++){
    imgsubcuadrante4[i]= (IMAGErgb*)malloc(sizeof(IMAGErgb)*(width/2));    
  }
  int h4 = height/2, w4 = width/2;
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
  cuadrante * subcuadrante4 = (cuadrante*)malloc(sizeof(cuadrante)*1);
  subcuadrante4->cuadrantePrincipal = imgsubcuadrante4;
  subcuadrante4->height = height/2;
  subcuadrante4->width = width/2;
  subcuadrante4->lvl = lvlPadre +1;
  subcuadrante4->subCuadrante1 = NULL;
  subcuadrante4->subCuadrante2 = NULL;
  subcuadrante4->subCuadrante3 = NULL;
  subcuadrante4->subCuadrante4 = NULL;
  structPadre->subCuadrante4 = subcuadrante4;


/*
0     w/2      w
1        2
0 0 0 0| 0 0 0 0 0      primer cuadrante : h = 0   -> h/2
0 0 0 0| 0 0 0 0                           w = 0   -> w/2
0 0 0 0| 0 0 0 0        Segundo cuadrante: h = 0   -> h/2
0 0 0 0| 0 0 0 0                           w = w/2 -> w
---------------- h/2    Tercer cuadrante : h = h/2 -> h
0 0 0 0| 0 0 0 0                           w = 0   -> w/2    
0 0 0 0| 0 0 0 0        Cuarto cuadrante : h = h/2 -> h
0 0 0 0| 0 0 0 0                           w = w/2 -> w
0 0 0 0| 0 0 0 0 h
3        4                                              */
};


IMAGErgb **LoadBMP(char *filename, bmpInfoHeader *bInfoHeader);
void DisplayInfo(bmpInfoHeader *info);
//void TextDisplay(bmpInfoHeader *info, unsigned char *img);
//./lab2 -i imagen_1.bmp -o histograma.txt -L 4 -B 6
int main(int argc, char** argv){
  int niveles,numerodeBins;
  char* archivoImagen,*archivoHistograma;
  int flag;
  char *mflag;
  int c;
  while (( (c = getopt(argc, argv, "i:o:L:B:d")) != -1)){
      switch (c)
      {
      case 'i':
      //
          mflag = optarg; 
      if(optarg == 0){
        printf("\nIngreso de parametro i incorrecto! \n");
              exit(EXIT_FAILURE);
      }else{
        archivoImagen = optarg;
      }
        break;
      case 'o':
      //
          mflag = optarg; 
          if(optarg == 0){
        printf("\nIngreso de parametro o incorrecto!\n");
              exit(EXIT_FAILURE);
      }else{
        archivoHistograma = optarg;
      }
        break;
    case 'L':
      //
      mflag = optarg; 
          if(optarg == 0){
        printf("\nIngreso de parametro L incorrecto!\n");
              exit(EXIT_FAILURE);
      }else{
        niveles = atof(optarg);
      }
        break;      
      case 'B':
      //
      if(optarg == 0){
        printf("\nIngreso de parametro B incorrecto!\n");
              exit(EXIT_FAILURE);
      }else{
        numerodeBins = atof(optarg);
      }
        break;
      case 'd':
      flag = 1;
      break;
      }
  }

  bmpInfoHeader info;
  IMAGErgb **img;

  img=LoadBMP(archivoImagen, &info);
  DisplayInfo(&info);
  //TextDisplay(&info, img);

  return 0;
}

/*void TextDisplay(bmpInfoHeader *info, unsigned char *img)
{
  int x, y;
  /* Reducimos la resolución vertical y horizontal para que la imagen entre en pantalla 
  static const int reduccionX=6, reduccionY=4;
  /* Si la componente supera el umbral, el color se marcará como 1. 
  static const int umbral=90;
  /* Asignamos caracteres a los colores en pantalla 
  static unsigned char colores[9]=" bgfrRGB";
  int r,g,b;

  /* Dibujamos la imagen 
  for (y=info->height; y>0; y-=reduccionY)
    {
      for (x=0; x<info->width; x+=reduccionX)
    {
      b=(img[3*(x+y*info->width)]>umbral);
      g=(img[3*(x+y*info->width)+1]>umbral);
      r=(img[3*(x+y*info->width)+2]>umbral);

      printf("%c", colores[b+g*2+r*4]);
    }
      printf("\n");
    }
}*/

IMAGErgb **LoadBMP(char *filename, bmpInfoHeader *bInfoHeader)
{

  FILE *f;
  bmpFileHeader header;     /* cabecera */
  //unsigned char *imgdata;   /* datos de imagen */
  uint16_t type;        /* 2 bytes identificativos */

  f=fopen (filename, "r");
  if (!f)
    return NULL;        /* Si no podemos leer, no hay imagen*/

  /* Leemos los dos primeros bytes */
  fread(&type, sizeof(uint16_t), 1, f);
  if (type !=0x4D42)        /* Comprobamos el formato */
    {
      fclose(f);
      return NULL;
    }

  /* Leemos la cabecera de fichero completa */
  fread(&header, sizeof(bmpFileHeader), 1, f);

  /* Leemos la cabecera de información completa */
  fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);
  /* Nos situamos en el sitio donde empiezan los datos de imagen,
   nos lo indica el offset de la cabecera de fichero*/
  fseek(f, header.offset, SEEK_SET);

  /* Reservamos memoria para la imagen, ¿cuánta?
     Tanto como indique imgsize */
  IMAGErgb **imgdata=(IMAGErgb**)malloc(sizeof(IMAGErgb*)*bInfoHeader->height);

  for(int i=0; i< bInfoHeader->height;i++){
    imgdata[i]= (IMAGErgb*)malloc(sizeof(IMAGErgb)*bInfoHeader->width);    
  }
  for(size_t i=0;i<bInfoHeader->height;i++){
    for(size_t j=0; j<bInfoHeader->width;j++){
      fread(&imgdata[i][j],1,sizeof(IMAGErgb),f);
    }
  }
  /* Cerramos */
  fclose(f);
  /*Print Matriz
  for(size_t i=0;i<bInfoHeader->height;i++){
    for(size_t j=0; j<bInfoHeader->width;j++){
      printf("[%d %d %d] ",imgdata[i][j].r,imgdata[i][j].g,imgdata[i][j].b);
    }
  }*/
  /* Devolvemos la imagen */
  return imgdata;
}

void DisplayInfo(bmpInfoHeader *info)
{
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