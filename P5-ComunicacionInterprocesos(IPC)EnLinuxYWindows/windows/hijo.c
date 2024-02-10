/* Programa hijo.c */
#include "windows.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "stdlib.h"
#include "unistd.h"


const int FILAS = 10;
const int COLUMNAS = 10;

void llenarmatriz(int m[][COLUMNAS]);
void imprimirmatriz(int m[][COLUMNAS]);
void imprimirmatrizres(int m[][COLUMNAS]);
void multiplicacion(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]);
void guardarmulti(int matrix[][COLUMNAS]);

int main()
{	
	srand(time(NULL));
	/*************** Se recibe el m1 y m2 del PADRE *************************/
	/************************************************************************/
	printf("\n 	PROCESO HIJO\n");
	DWORD leidos;	
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	SECURITY_ATTRIBUTES pipeSeg = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
	/* Lectura desde la tubería sin nombre */
	
	int matriz1[FILAS][COLUMNAS];
	int matriz2[FILAS][COLUMNAS];

	ReadFile(hStdIn, matriz1, 100*sizeof(int), &leidos, NULL);
	ReadFile(hStdIn, matriz2, 100*sizeof(int), &leidos, NULL);

	printf("Matriz 1 y 2 recibidas del proceso PADRE.\n");
	printf("Matriz 1:\n");
	imprimirmatriz(matriz1); //impresion m1
	printf("\n");
	printf("Matriz 2:\n");
	imprimirmatriz(matriz2); //impresion m2
	printf("\n");
	int mr[FILAS][COLUMNAS];
	multiplicacion(matriz1,matriz2,mr); //Se hace la multiplicacion
	printf("Multiplicacion:\n");
	imprimirmatrizres(mr); //Se imprime la matriz resultado
	
	
	CloseHandle(hStdIn);

	/*************** Se crea el proceso nieto hijo del hijo *****************/
	/************************************************************************/
	
	DWORD escritosHN;												 					//Se usa para obtener cuantos caracteres fueron escritos dentro del pipe
	HANDLE hLecturaPipeHN,hEscrituraPipeHN; //H-N	HIJO-NIETO						//Manejadores para escritura y lectura de una pipe
	PROCESS_INFORMATION piHijoHN;														//almacena la informacion relacionada con el proceso que se crea
	STARTUPINFO siHijoHN; 															// Almacena la información de inicio del proceso hijo
																					//se va a utilizar para inicializar atributos de seguridad relacionados con la pipe que se creara
	SECURITY_ATTRIBUTES pipeSegHN = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE}; 

	//Obtencion de información para la inicialización del proceso hijo
	GetStartupInfo(&siHijoHN);	
	//Cración de la tubería sin nombre
	CreatePipe(&hLecturaPipeHN, &hEscrituraPipeHN, &pipeSegHN,0);
	//Escritura en la tuberia sin nombre

	int matriz3[FILAS][COLUMNAS];
	int matriz4[FILAS][COLUMNAS];
	llenarmatriz(matriz3);
	llenarmatriz(matriz4);

	WriteFile(hEscrituraPipeHN, matriz3,100*sizeof(int), &escritosHN, NULL);
	WriteFile(hEscrituraPipeHN, matriz4,100*sizeof(int), &escritosHN, NULL);

	printf("Proceso NIETO creado.\n");
	printf("Matriz 3 y Matriz 4 enviadas para sumar.\n");
	

	siHijoHN.hStdInput = hLecturaPipeHN; 												//se cambia la entrada estandar del hijo
	siHijoHN.hStdError = GetStdHandle (STD_ERROR_HANDLE);	
	siHijoHN.hStdOutput = GetStdHandle (STD_OUTPUT_HANDLE);
	siHijoHN.dwFlags = STARTF_USESTDHANDLES;											//indica que se usaran las banderas estandares cuando se cree el proceso hijo
	CreateProcess (NULL, "nieto.exe", NULL, NULL,
		TRUE, // Hereda el proceso hijo los manejadores del padre 
		0, NULL, NULL, &siHijoHN, &piHijoHN);

	//concluye el proceso padre con el closeHandle
	//de todos los manejadores que se usaron
	WaitForSingleObject (piHijoHN.hProcess, INFINITE);

	CloseHandle(hLecturaPipeHN);
	CloseHandle(hEscrituraPipeHN);
	CloseHandle(piHijoHN.hThread);
	CloseHandle(piHijoHN.hProcess);

	printf("Matriz Resultado de la Multiplicacion enviada al proceso PADRE.\n");
	printf("Termina el proceso HIJO, continua el proceso PADRE\n");
	guardarmulti(mr);
	
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
void multiplicacion(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]){
	int a, i, j;
	for(a = 0;a < FILAS;a++){
		for(i = 0; i < FILAS; i++){
			int suma = 0;	
			for(j = 0;j < FILAS; j++){
				suma +=(m1[i][j] * m2[j][a]);
			}
			mr[i][a] = suma;			
		}		
	}
}
void llenarmatriz(int matriz[][COLUMNAS]){
	int i, j;
	for (i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){		
			matriz[i][j] = rand() % 10;
		}
	}
}
void guardarmulti(int matrix[][COLUMNAS]){
	FILE *m = NULL;	
	m = fopen("MatrizMultiplicacion.txt","w+");
	if(m == NULL){
		printf("No fue posible abrir el archivo\n");
	}
	int i, j;
	for (i = 0; i < FILAS; i++){
		//fprintf(m,"");
		for(j = 0; j < COLUMNAS; j++){			
			fprintf(m,"<%d>	",matrix[i][j]);
			if(j == 9){
				fprintf(m,".\n");	
			}		
		}
	}
	fclose(m);//Se cierra el archivo
}

