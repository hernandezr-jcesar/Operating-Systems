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
size_t getline(char **lineptr, size_t *n, FILE *stream);
void imprimirmatriz(int matriz[][COLUMNAS]);
void concatenar(char* cad, char caracter);
void suma(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]);
void guardarmatriz(int matrix[][COLUMNAS], char* nombre);

int main()
{
	printf("\n			PROCESO NIETO\n");
	int matriz1[FILAS][COLUMNAS];		
	leermatriz(matriz1,"matriz1.txt");
	printf("			Matriz 1:\n");
	imprimirmatriz(matriz1);

	int matrizdos[FILAS][COLUMNAS];
	leermatriz(matrizdos,"matriz2.txt");
	printf("			Matriz 2:\n");
	imprimirmatriz(matrizdos);

	printf("			Matriz resultado de la suma:\n");
	int mr[FILAS][COLUMNAS];
	suma(matriz1,matrizdos,mr);
	imprimirmatriz(mr);
	guardarmatriz(mr,"Suma.txt");

	printf("			Matriz resultado enviada al padre\n");
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
void imprimirmatriz(int matriz[][COLUMNAS]){
	for (i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){
			if(j == 0){
				printf("			");	
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
void suma(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]){
	int i, j;
	for(i = 0; i < FILAS; i++){
		for(j = 0;j < FILAS; j++){		
			mr[i][j] = m1[i][j] + m2[i][j];				
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
