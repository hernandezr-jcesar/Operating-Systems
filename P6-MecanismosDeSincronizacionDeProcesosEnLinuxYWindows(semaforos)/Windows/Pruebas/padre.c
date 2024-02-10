#include <windows.h> /*Programa padre*/
#include <stdio.h>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const int FILAS = 10;
const int COLUMNAS = 10;

#define TAM_MEM 400
int i, j;

void guardarmatriz(int matrix[][COLUMNAS], char* nombre);
void llenarmatriz(int matriz[][COLUMNAS]);
void imprimirmatriz(int matriz[][COLUMNAS]);
void leermatriz(int matrix[][COLUMNAS], char* nombre);
void cadenaAmatriz(char* cad,int matriz[][COLUMNAS]);
size_t getline(char **lineptr, size_t *n, FILE *stream);
void copiarmatriz(int m1[][COLUMNAS],float mr[][COLUMNAS]);
void inversa(float matrix[][COLUMNAS]);
void imprimirmatrizres_float(float matriz[][COLUMNAS]);
void guardarmulti(float matrix[][COLUMNAS]);
void guardarsuma(float matrix[][COLUMNAS]);
void concatenar(char* cad, char caracter);

int main(int argc, char *argv[])
{
	remove("matriz1.txt");
	remove("matriz2.txt");
	remove("Multiplicacion.txt");
	remove("Suma.txt");
	remove("MatrizInversaSuma.txt");
	remove("MatrizInversaMultiplicacion.txt");
	srand(time(NULL));
	STARTUPINFO si; /* Estructura de información inicial para Windows */
	PROCESS_INFORMATION pi; /* Estructura de información del adm. de procesos */	

	HANDLE hSemaforo;
	int i=1;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	//Se verifica que se ejecute bien el programa
	if(argc!=2)
	{
		printf("Usar: %s Nombre_programa_hijo\n", argv[0]);
		return;
	}
	// Creación del semáforo
	if((hSemaforo = CreateSemaphore(NULL, 1, 1, "Semaforo")) == NULL)
	{
		printf("Falla al invocar CreateSemaphore: %d\n", GetLastError());
		return -1;
	}
	// Creación proceso hijo
	if(!CreateProcess(NULL, argv[1], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		printf("Falla al invocar CreateProcess: %d\n", GetLastError() );
		return -1;
	}
	while(i<2)
	{
		// Prueba del semáforo
		WaitForSingleObject(hSemaforo, INFINITE);
		//Sección crítica
		printf("Soy el padre entrando al semaforo\n",i);
		//Sleep(5000);
		{//Codigo de creacion de memoria compartida 
			printf("\n	PROCESO PADRE\n");
			int matriz1[FILAS][COLUMNAS];
			int matriz2[FILAS][COLUMNAS];
			llenarmatriz(matriz1);
			llenarmatriz(matriz2);
			guardarmatriz(matriz1,"matriz1.txt");
			guardarmatriz(matriz2,"matriz2.txt");
			printf("	Matriz 1:\n");
			imprimirmatriz(matriz1);
			printf("	Matriz 2:\n");
			imprimirmatriz(matriz2);
			printf("	Matrices enviadas para multiplicar\n");
		}
		//Liberación el semáforo
		if (!ReleaseSemaphore(hSemaforo, 1, NULL) )
		{
			printf("Falla al invocar ReleaseSemaphore: %d\n", GetLastError());
		}
		printf("Soy el padre liberando al semaforo\n",i);
		Sleep(1000);
		i++;
	}

	// Terminación controlada del proceso e hilo asociado de ejecución
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	{
		printf("\n 	PROCESO PADRE\n");
		int m_multiplicacion[FILAS][COLUMNAS];
		leermatriz(m_multiplicacion,"Multiplicacion.txt");
		printf("Matriz resultado de la Multiplicacion\n");
		imprimirmatriz(m_multiplicacion);

		int m_suma[FILAS][COLUMNAS];
		leermatriz(m_suma,"Suma.txt");
		printf("Matriz resultado de la Suma\n");
		imprimirmatriz(m_suma);

		//Sacar las inversas de estas matrices resultado
		float m_inversa_multi[FILAS][COLUMNAS];
		copiarmatriz(m_multiplicacion,m_inversa_multi);
		inversa(m_inversa_multi);

		float m_inversa_suma[FILAS][COLUMNAS];
		copiarmatriz(m_suma,m_inversa_suma);
		inversa(m_inversa_suma);

		printf("\n");
		printf("Inversa de la multiplicacion:\n");
		imprimirmatrizres_float(m_inversa_multi);
		printf("\n");
		printf("Inversa de la suma:\n");
		imprimirmatrizres_float(m_inversa_suma);
		
		guardarmulti(m_inversa_multi);
		guardarsuma(m_inversa_suma);

		printf("Archivos creados.\nFin del programa.\n");
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
void imprimirmatriz(int matriz[][COLUMNAS]){
	for (i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){
			if(j == 0){
				printf("	");	
			}
			printf("%d ",matriz[i][j]);
			if(j == 9){
				printf("\n");	
			}		
		}
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
void copiarmatriz(int m1[][COLUMNAS],float mr[][COLUMNAS]){
	int i, j;
	for (i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){	
			int temp = m1[i][j];
			mr[i][j] = (float)temp;		
		}
	}	
}
void inversa(float matrix[][COLUMNAS]){
	float **A,**I,temp;
    int i,j,k,matsize;

    matsize = FILAS;

    A=(float **)malloc(matsize*sizeof(float *));           							   //allocate memory dynamically for matrix A(matsize X matsize)
	
    for(i=0;i<matsize;i++)
        A[i]=(float *)malloc(matsize*sizeof(float));

    I=(float **)malloc(matsize*sizeof(float *));        					           //memory allocation for indentity matrix I(matsize X matsize)

    for(i=0;i<matsize;i++)
        I[i]=(float *)malloc(matsize*sizeof(float));

    // se cargan los elementos de la matriz a A
    for(i=0;i<matsize;i++)
        for(j=0;j<matsize;j++)
        	A[i][j] = matrix[i][j];            


    for(i=0;i<matsize;i++)                                  					       //automatically initialize the unit matrix, e.g.
        for(j=0;j<matsize;j++)                             						       //  -       -
            if(i==j)                                        					   		// | 1  0  0 |
                I[i][j]=1;                                  							// | 0  1  0 |
            else                                           	 							// | 0  0  1 |
                I[i][j]=0;                                  							//  -       -
/*---------------LoGiC starts here------------------*/      							//procedure // to make the matrix A to unit matrix

    for(k=0;k<matsize;k++)                                 								 //by some row operations,and the same row operations of
    {                                                      								 //Unit mat. I gives the inverse of matrix A
        temp=A[k][k];                  					 								//'temp'  
        																				// stores the A[k][k] value so that A[k][k]  will not change
        for(j=0;j<matsize;j++)     														//during the operation //A[i] //[j]/=A[k][k]  when i=j=k
        {
            A[k][j]/=temp;                                 								 //it performs // the following row operations to make A to unit matrix
            I[k][j]/=temp;                                  							//R0=R0/A[0][0],similarly for I also R0=R0/A[0][0]
        }                                                   							//R1=R1-R0*A[1][0] similarly for I
        for(i=0;i<matsize;i++)                              							//R2=R2-R0*A[2][0]      ,,
        {
            temp=A[i][k];                      											 //R1=R1/A[1][1]
            for(j=0;j<matsize;j++)             											//R0=R0-R1*A[0][1]
            {                                   										//R2=R2-R1*A[2][1]
                if(i==k)
                    break;                      										//R2=R2/A[2][2]
                A[i][j]-=A[k][j]*temp;          										//R0=R0-R2*A[0][2]
                I[i][j]-=I[k][j]*temp;          										//R1=R1-R2*A[1][2]
            }
        }
    }
/*---------------LoGiC ends here--------------------*/
    //printf("The inverse of the matrix is: ");               							//Print the //matrix I that now contains the inverse of mat. A
    for(i=0;i<matsize;i++)
    {
        for(j=0;j<matsize;j++)
        	matrix[i][j] = I[i][j];            
    }
    
}
void imprimirmatrizres_float(float matriz[][COLUMNAS]){
	int i, j;
	for (i = 0; i < FILAS; i++){
		printf("	");
		for(j = 0; j < COLUMNAS; j++){			
			printf("%0.4f	",matriz[i][j]);
			if(j == 9){
				printf("  \n");	
			}		
		}
	}
}
void guardarmulti(float matrix[][COLUMNAS]){
	FILE *m1 = NULL;	
	m1 = fopen("MatrizInversaMultiplicacion.txt","w+");
	if(m1 == NULL){
		printf("No fue posible abrir el archivo\n");
	}
	fprintf(m1,"Matriz inversa de la Multiplicacióń:\n");
	int i, j;
	for (i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){			
			fprintf(m1,"%0.4f	",matrix[i][j]);
			if(j == 9){
				fprintf(m1,"	\n");	
			}		
		}
	}
	fclose(m1);//Se cierra el archivo
}
void guardarsuma(float matrix[][COLUMNAS]){
	FILE *m = NULL;	
	m = fopen("MatrizInversaSuma.txt","w+");
	if(m == NULL){
		printf("No fue posible abrir el archivo\n");
	}
	fprintf(m,"Matriz inversa de la Suma:\n");
	int i, j;
	for (i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){			
			fprintf(m,"%0.4f	",matrix[i][j]);
			if(j == 9){
				fprintf(m,"	\n");	
			}		
		}
	}
	fclose(m);//Se cierra el archivo
}
void concatenar(char* cad, char caracter){		
	char cadTemp[2];
	cadTemp[0] = caracter;
	cadTemp[1] = '\0';
	strcat(cad,cadTemp);							
}
