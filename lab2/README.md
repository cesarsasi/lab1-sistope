Laboratorio 2
Profesor   : Fernando Rannou
Ayudante   : Benjamin Muñoz
Integrantes:    
-				Octavio Nicolas Canales Ñirriman 20.003.610-7
-				César Antonio Salazar Silva      19.916.471-6

Sistemas Operativos 2.2020  (Creación: 01-Noviembre-2020)

Este entregable corresponde al desarrollo del laboratorio 1 del ramo Sistemas Operativos.
Para compilar el programa debe abrir consola y ejecutar los siguientes comandos: 
- make
Ejemplos de ejecucion del programa:
Formato de de entrega de parametros = ./lab2 -i [string nombre .bmp ejemplo] -o [string nombre archivo al que dejar resultados]-L [entero para cantidad de niveles] -B [entero cantidad de bins] -d [flag para imprimir por consola]
Ejemplos de prueba (OBS: -L y -B deben ser enteros potencia de dos, -L entero de 0 a 9 y -B entero de 0 a 256):
1) ./lab2 -i imagen_1.bmp -o histograma.txt -L 4 -B 4 
2) ./lab2 -i imagen_1.bmp -o histograma.txt -L 4 -B 4 -d