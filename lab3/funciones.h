#include <math.h>

void escrituraResultados(char * archivoFinal, int idDisco, int mediaReal, int mediaImag, int potencia, int ruidoTotal){
    FILE* fileFinal = fopen(archivoFinal, "a");
    fprintf(fileFinal,"Disco: %d",idDisco);
    fprintf(fileFinal,"Media real: %d",mediaReal);
    fprintf(fileFinal,"Media imaginaria: %d",mediaImag);
    fprintf(fileFinal,"Potencia: %d",potencia);
    fprintf(fileFinal,"Ruido total %d:",ruidoTotal);
}
0 u
1 v
2 r
3 i
4 w
float calculoPotenciaParcial(float ** matriz, cantidad){
    float potenciaParcial = 0;
    for (int i = 0; i < cantidad; ++i){
        potenciaParcial += pow(matriz[i][2],2) + sqrt(pow(matriz[i][3],2));
    }
    return potenciaParcial;
}

float calculoRuidoTotalParcial(float ** matriz, cantidad){
    float ruidoTotalParcial = 0;
    for (int i = 0; i < cantidad; ++i){
        potenciaParcial += matriz[i][4];
    }
    return potenciaParcial;
    
}