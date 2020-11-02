//Includes
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

//Funciones

//Main programa coordinador
int main(int argc, char** argv){
    char nombreArchivo[];
    int cantProcesos;//                              Procesos no puede ser 0 !!!!!!!
    int cantLineas;
    char cadenaBuscar[];



    //Proceso Coordinador

    //Calculo de lineas por proceso
    int lineasporProcesos = cantLineas/cantProcesos;
    int diferenciaLineProce = cantLineas - lineasporProcesos*cantProcesos;
    if(diferenciaLineProce == 0 ){//Las lineas se distribuyen equitativamente en cantidad divLineProce
        for (int  i = 0; i < cantProcesos; i++){
            //dar (lineasporProceso)
        }
        
    }else if(diferenciaLineProce < 0){//El ultimo proceso queda con menos lineas
        for (int i = 0; i < cantProcesos -1; i++){
            //dar (lineasporProceso)
        }
        //dar procesos restantes (diferenciaLineProce)
    }else if(diferenciaLineProce > 0){//El ultimo proceso queda con (diferenciaLineProce) mas lineas.
        for (int i = 0; i < cantProcesos -1; i++){
            //dar (lineasporProceso)
        }
        //dar procesos restantes (diferenciaLineProce)
    }

}