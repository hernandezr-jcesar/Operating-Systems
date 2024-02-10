#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

const int FILAS = 10;
const int COLUMNAS = 10;
#define LONGITUD_MAXIMA_CADENA 1000

void leermatrizdearchivo(int matriz[][COLUMNAS], int n);
void imprimirmatrizres(int matriz[][COLUMNAS]);
void concatenar(char* cad1, char caracter);

int main(){
	
	int matriz[FILAS][COLUMNAS];
	leermatrizdearchivo(matriz,0);
	printf("Matriz:\n");
	imprimirmatrizres(matriz);
	
	return 0;
}
void leermatrizdearchivo(int matriz[][COLUMNAS], int n){
	if(n == 0){
		char cadena[10] = "";		
		int fila = 0;
		int columna = 0;
		FILE *archivo = fopen("MatrizMultiplicacion.txt", "r"); // Modo lectura
	    char bufer[LONGITUD_MAXIMA_CADENA];         // Aquí vamos a ir almacenando cada línea
	    while (fgets(bufer, LONGITUD_MAXIMA_CADENA, archivo)){		
	        //printf("%s\n", bufer);
	        int i;	        
	        for(i = 0; i < strlen(bufer);i++){	
				concatenar(cadena,bufer[i]);				
				if(bufer[i] == '<'){
					memset(cadena,'\0',strlen(cadena));													
				}				
				if(bufer[i] == '.'){
					columna = 0;
				}
				if(bufer[i] == '>'){	
					int num = atoi(cadena);				
					matriz[fila][columna] = num;
					memset(cadena,'\0',strlen(cadena));	
					columna++;												
				}
																													
			}	        
	        fila++;
	    }		
	}
		
}
void concatenar(char* cad, char caracter){		
	char cadTemp[2];
	cadTemp[0] = caracter;
	cadTemp[1] = '\0';
	strcat(cad,cadTemp);							
}
void imprimirmatrizres(int matriz[][COLUMNAS]){
	int a, i, j;
	for (i = 0; i < FILAS; i++){		
		printf("	");				
		for(j = 0; j < COLUMNAS; j++){			
			printf("	%d",matriz[i][j]);
			if(j == 9){
				printf("\n");	
			}		
		}
	}
}

