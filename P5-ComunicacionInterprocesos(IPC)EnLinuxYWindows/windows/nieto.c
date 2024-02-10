/* Programa nieto.c */
#include "windows.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "stdlib.h"
#include "unistd.h"

const int FILAS = 10;
const int COLUMNAS = 10;

void imprimirmatriz(int m[][COLUMNAS]);
void imprimirmatrizres(int m[][COLUMNAS]);
void suma(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]);
void guardarsuma(int matrix[][COLUMNAS]);

int main()
{
	printf("\n 	PROCESO NIETO\n");
	char mensaje[30];
	DWORD leidos;
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	SECURITY_ATTRIBUTES pipeSeg = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
	/* Lectura desde la tubería sin nombre */

	int matriz3[FILAS][COLUMNAS];
	int matriz4[FILAS][COLUMNAS];

	ReadFile(hStdIn, matriz3, 100*sizeof(int), &leidos, NULL);
	ReadFile(hStdIn, matriz4, 100*sizeof(int), &leidos, NULL);

	printf("Matriz 3 y 4 recibidas del proceso HIJO.\n");
	printf("Matriz 3:\n");
	imprimirmatriz(matriz3); //impresion m1
	printf("\n");
	printf("Matriz 4:\n");
	imprimirmatriz(matriz4); //impresion m2
	printf("\n");
	int mr[FILAS][COLUMNAS];
	suma(matriz3,matriz4,mr); //Se hace la multiplicacion
	printf("Suma:\n");
	imprimirmatrizres(mr); //Se imprime la matriz resultado

	CloseHandle(hStdIn);

	printf("\nMatriz Resultado de la Suma enviada al proceso PADRE\n");
	printf("Termina el proceso NIETO, continua el proceso HIJO\n");
	guardarsuma(mr);

	return 0;
}
void imprimirmatriz(int matriz[][COLUMNAS]){
	int i, j;
	for (i = 0; i < FILAS; i++){		
		printf("	");				
		for(j = 0; j < COLUMNAS; j++){
			
			printf("%d  ",matriz[i][j]);
			if(j == 9){
				printf("	\n");	
			}		
		}
	}
}
//imprimir matrices resultado
void imprimirmatrizres(int matriz[][COLUMNAS]){
	int i, j;
	for (i = 0; i < FILAS; i++){		
		printf("	");				
		for(j = 0; j < COLUMNAS; j++){
			
			printf("%d	",matriz[i][j]);
			if(j == 9){
				printf("	\n");	
			}		
		}
	}
}
void suma(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]){
	int i, j;
	for(i = 0; i < FILAS; i++){
		for(j = 0;j < FILAS; j++){		
			mr[i][j] = m1[i][j] + m2[i][j];				
		}
	}
}
void guardarsuma(int matrix[][COLUMNAS]){
	FILE *m = NULL;	
	m = fopen("MatrizSuma.txt","w+");
	if(m == NULL){
		printf("No fue posible abrir el archivo\n");
	}
	int i, j;
	for (i = 0; i < FILAS; i++){
		//fprintf(m,"	|");
		for(j = 0; j < COLUMNAS; j++){			
			fprintf(m,"<%d> 	",matrix[i][j]);
			if(j == 9){
				fprintf(m,".\n");	
			}		
		}
	}
	fclose(m);//Se cierra el archivo
}

