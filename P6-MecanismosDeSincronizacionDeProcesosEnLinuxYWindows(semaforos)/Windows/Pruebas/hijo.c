#include <windows.h> /*Programa hijo*/
#include <stdio.h>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const int FILAS = 10;
const int COLUMNAS = 10;

#define TAM_MEM 400
int i, j;

void leermatriz(int matrix[][COLUMNAS], char* nombre);
void cadenaAmatriz(char* cad,int matriz[][COLUMNAS]);
void imprimirmatriz(int matriz[][COLUMNAS]);
void concatenar(char* cad, char caracter);
void multiplicacion(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]);
void guardarmatriz(int matrix[][COLUMNAS], char* nombre);
size_t getline(char **lineptr, size_t *n, FILE *stream);


int main()
{
	HANDLE hSemaforo;
	int i=1;
	// Apertura del semáforo
	if((hSemaforo = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Semaforo")) ==NULL)
	{
		printf("Falla al invocar OpenSemaphore: %d\n", GetLastError());
		return -1;
	}
	while(i<2)
	{
		// Prueba del semáforo
		WaitForSingleObject(hSemaforo, INFINITE);
		//Sección crítica
		printf("Soy el hijo entrando al semaforo\n",i);
		//Sleep(5000);
		{//Codigo del acceso a la memoria compartida
			printf("\n		PROCESO HIJO\n");
			int matriz1[FILAS][COLUMNAS];		
			leermatriz(matriz1,"matriz1.txt");
			printf("		Matriz 1:\n");
			imprimirmatriz(matriz1);

			int matrizdos[FILAS][COLUMNAS];
			leermatriz(matrizdos,"matriz2.txt");
			printf("		Matriz 2:\n");
			imprimirmatriz(matrizdos);

			printf("		Matriz resultado de la multiplicacion\n");
			int matriz_res_multi[FILAS][COLUMNAS];
			multiplicacion(matriz1,matrizdos,matriz_res_multi);
			imprimirmatriz(matriz_res_multi);
			guardarmatriz(matriz_res_multi,"Multiplicacion.txt");
			printf("		Matriz resultado enviada al padre\n");
			

		}
		//Liberación el semáforo
		if (!ReleaseSemaphore(hSemaforo, 1, NULL) )
		{
			printf("Falla al invocar ReleaseSemaphore: %d\n", GetLastError());
		}
		printf("Soy el hijo liberando al semaforo\n",i);
		//Sleep(5000);
		{//CREACION DEL PROCESO NIETO
				PROCESS_INFORMATION piHijoHN;
				STARTUPINFO siHijoHN;
				//Obtencion de información para la inicialización del proceso hijo
				GetStartupInfo(&siHijoHN);	
				printf("		Proceso NIETO creado.\n");
				printf("		Matrices enviadas para sumar.\n");
				CreateProcess (NULL, "nieto.exe", NULL, NULL,
				TRUE, // Hereda el proceso hijo los manejadores del padre 
				0, NULL, NULL, &siHijoHN, &piHijoHN);
				WaitForSingleObject (piHijoHN.hProcess, INFINITE);
				CloseHandle(piHijoHN.hThread);
				CloseHandle(piHijoHN.hProcess);
		}
		i++;
	}
}
void leermatriz(int matrix[][COLUMNAS], char* nombre){	
	FILE* input_file = fopen(nombre, "r");	
	if(!input_file)
		exit(EXIT_FAILURE);

	char *linea = NULL;
	size_t len = 0;
	
	char buffer[500];	
	while(getline(&linea, &len, input_file) != -1){
		//printf("%s", linea);
		strcat(strcpy(buffer,buffer), linea);		
	}
	//printf("\n\n%s\n",buffer);
	cadenaAmatriz(buffer,matrix);
	
	fclose(input_file);
	free(linea);
}
void cadenaAmatriz(char* cad,int matriz[][COLUMNAS]){	
	char numero[12] = "";
	int number;
	int i,j,k;
	int fila = 0;
	int columna = 0;	
	for(i = 0; i < strlen(cad); i++){
		concatenar(numero,cad[i]);		
		if(cad[i] == '<'){
			columna = 0;			
			memset(numero,'\0',strlen(numero));
		}
		if(cad[i] == '|'){
			number = atoi(numero);
			matriz[fila][columna] = number ;
			memset(numero,'\0',strlen(numero));
			columna++;
		}
		if(cad[i] == '>'){			
			fila++;			
		}
	}
}
void imprimirmatriz(int matriz[][COLUMNAS]){
	for (i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){
			if(j == 0){
				printf("		");	
			}
			printf("%d ",matriz[i][j]);
			if(j == 9){
				printf("\n");	
			}		
		}
	}
}
void concatenar(char* cad, char caracter){
	char cadTemp[2];
	cadTemp[0] = caracter;
	cadTemp[1] = '\0';
	strcat(cad,cadTemp);
}
void multiplicacion(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]){
	int a;
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
void guardarmatriz(int matrix[][COLUMNAS], char* nombre){
	FILE *archivo = NULL;	
	archivo = fopen(nombre,"w+");
	if(archivo == NULL){
		printf("No fue posible abrir el archivo\n");
	}	
	for (i = 0; i < FILAS; i++){
		fprintf(archivo,"<");	
		for(j = 0; j < COLUMNAS; j++){								
			fprintf(archivo,"%d|",matrix[i][j]);																							
		}
		fprintf(archivo,">\n");	
	}
	fclose(archivo);//Se cierra el archivo
}
size_t getline(char **lineptr, size_t *n, FILE *stream){
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}
