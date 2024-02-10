#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const int FILAS = 10;
const int COLUMNAS = 10;
const int TAM_NMEM = 120*sizeof(int);

void llenarmatriz(int matriz[][COLUMNAS]);
void imprimirmatriz(int matriz[][COLUMNAS]);
void matrizatexto(int matriz[][COLUMNAS], char* cad);
void cadenaAentero(char* cad,int matriz[][COLUMNAS]);
void concatenar(char* cad, char caracter);

int main(void){
	srand(time(NULL));
	pid_t pid;

	if((pid = fork()) == 0){
		//PROCESO HIJO
		printf("Soy el hijo(%d, hijo de %d)\n",getpid(), getppid());
		//Matriz 1 
		int matriz1[FILAS][COLUMNAS];

		int shmidM1;   //Para almacenar identificador de la memoria compartida
		key_t llaveM1; //Valor numerico para identificar a la memoria compartida
		char *shmM1, *sM1;
		llaveM1 = 100; // Valor entero
		if((shmidM1 = shmget(llaveM1,TAM_NMEM,0666)) < 0){
			perror("Error al obtener memoria compartida: shmget");
			exit(-1);
		}
		if((shmM1 = shmat(shmidM1,NULL,0)) == (char*)-1){
			perror("Error al enlazar la memoria compartida; shmat");
			exit(-1);
		} 
		
		char cadena1[250] = "";
		//char c[1];
		sM1 = shmM1;
		strcpy(cadena1,sM1);
		printf("String: \n%s\n\n",sM1);
		//printf("Cadena: \n%s\n\n",cadena);
		//for(sM1 = shmM1; *sM1 != '\0';sM1++)
		//	putchar(*sM1);		

		cadenaAentero(cadena1,matriz1);
		imprimirmatriz(matriz1);

		*shmM1 = '*';	
		
		//Matriz 2		
		int matriz2[FILAS][COLUMNAS];	

		int shmidM2;   //Para almacenar identificador de la memoria compartida
		key_t llaveM2; //Valor numerico para identificar a la memoria compartida
		char *shmM2, *sM2;
		llaveM2 = 200; // Valor entero
		if((shmidM2 = shmget(llaveM2,TAM_NMEM,IPC_CREAT|0666)) < 0){
			perror("Error al obtener memoria compartida: shmget");
			exit(-1);
		}
		if((shmM2 = shmat(shmidM2,NULL,0)) == (char*)-1){
			perror("Error al enlazar la memoria compartida; shmat");
			exit(-1);
		}
		char cadena2[250] = "";	
		sM2 = shmM2;
		strcpy(cadena2,sM2);
		printf("String2: \n%s\n\n",sM2);

		cadenaAentero(cadena2,matriz2);
		imprimirmatriz(matriz2);

		*shmM2 = '*';
		exit(0);
	}
	else{
		//PROCESO PADRE
		printf("Soy el padre(%d, hijo de %d)\n",getpid(), getppid());	
			
		//Matriz 1 

		int matriz1[FILAS][COLUMNAS];
		llenarmatriz(matriz1);
		imprimirmatriz(matriz1);

		int shmidM1;   //Para almacenar identificador de la memoria compartida
		key_t llaveM1; //Valor numerico para identificar a la memoria compartida
		char *shmM1, *sM1;
		llaveM1 = 100; // Valor entero
		if((shmidM1 = shmget(llaveM1,TAM_NMEM,IPC_CREAT|0666)) < 0){
			perror("Error al obtener memoria compartida: shmget");
			exit(-1);
		}
		if((shmM1 = shmat(shmidM1,NULL,0)) == (char*)-1){
			perror("Error al enlazar la memoria compartida; shmat");
			exit(-1);
		} 
		
		sM1 = shmM1;
		matrizatexto(matriz1,sM1);
		for(sM1 = shmM1; *sM1 != '\0';sM1++)
			putchar(*sM1);		
		putchar('\n');
			
		//while(*shmM1!='*')
			//sleep(1); //El cliente es el que escribe el a
		//exit(0);

		//Matriz 2

		int matriz2[FILAS][COLUMNAS];
		llenarmatriz(matriz2);
		imprimirmatriz(matriz2);
		
		int shmidM2;   //Para almacenar identificador de la memoria compartida
		key_t llaveM2; //Valor numerico para identificar a la memoria compartida
		char *shmM2, *sM2;
		llaveM2 = 200; // Valor entero
		if((shmidM2 = shmget(llaveM2,TAM_NMEM,IPC_CREAT|0666)) < 0){
			perror("Error al obtener memoria compartida: shmget");
			exit(-1);
		}
		if((shmM2 = shmat(shmidM2,NULL,0)) == (char*)-1){
			perror("Error al enlazar la memoria compartida; shmat");
			exit(-1);
		}

		sM2 = shmM2;
		matrizatexto(matriz2,sM2);

		for(sM2 = shmM2; *sM2 != '\0';sM2++)
			putchar(*sM2);		
		putchar('\n');
			
		while(*shmM2!='*')
			sleep(1); //El cliente es el que escribe el a
		exit(0);
	}
	return 0;

}
void llenarmatriz(int matriz[][COLUMNAS]){
	for (int i = 0; i < FILAS; i++){
		for(int j = 0; j < COLUMNAS; j++){		
			matriz[i][j] = rand() % 10;
		}
	}
}
void imprimirmatriz(int matriz[][COLUMNAS]){
	for (int i = 0; i < FILAS; i++){
		for(int j = 0; j < COLUMNAS; j++){
			if(j == 0){
				//printf("|   ");	
			}
			printf("%d ",matriz[i][j]);
			if(j == 9){
				printf("  \n");	
			}		
		}
	}
}
void matrizatexto(int matriz[][COLUMNAS], char* cad){
	int numero;
	char charnumero[12];
	char c;
	int i, j, k;
	for(i = 0; i < FILAS; i++){
		*cad++ = '<';
		for(j = 0; j < COLUMNAS; j++){
			numero = matriz[i][j];			
			sprintf(charnumero,"%d", numero);
			k = 0;	
			for(k = 0; k < strlen(charnumero);k++){
				c = charnumero[k];
				*cad++ = c;				
				//printf("%c",charnumero[k]);
			}		
			*cad++ = '|';
		}
		*cad++ = '>';
	}
	*cad = '\0';
}
void cadenaAentero(char* cad,int matriz[][COLUMNAS]){	
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
void concatenar(char* cad, char caracter){
	char cadTemp[2];
	cadTemp[0] = caracter;
	cadTemp[1] = '\0';
	strcat(cad,cadTemp);
}