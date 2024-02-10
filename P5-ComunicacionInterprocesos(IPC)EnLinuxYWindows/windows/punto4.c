#include "windows.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
#include "stdlib.h"
#include "unistd.h"

const int FILAS = 10;
const int COLUMNAS = 10;
#define LONGITUD_MAXIMA_CADENA 1000


void llenarmatriz(int m[][COLUMNAS]);
void imprimirmatrizres(int matriz[][COLUMNAS]);
void leermatrizdearchivo(int matriz[][COLUMNAS], int n);
void concatenar(char* cad, char caracter);
void imprimirmatrizres_float(float matriz[][COLUMNAS]);
void copiarmatriz(int m1[][COLUMNAS],float mr[][COLUMNAS]);
void inversa(float matrix[][COLUMNAS]);
void guardarmulti(float matrix[][COLUMNAS]);
void guardarsuma(float matrix[][COLUMNAS]);

int main(int argc, char *argv[])
{
	srand(time(NULL));

	printf("\n 	PROCESO PADRE\n");
	int matriz1[FILAS][COLUMNAS];
	int matriz2[FILAS][COLUMNAS];
	llenarmatriz(matriz1);
	llenarmatriz(matriz2);

	DWORD escritos;												 					//Se usa para obtener cuantos caracteres fueron escritos dentro del pipe
	HANDLE hLecturaPipe,hEscrituraPipe; 											//Manejadores para escritura y lectura de una pipe
	PROCESS_INFORMATION piHijo;														//almacena la informacion relacionada con el proceso que se crea
	STARTUPINFO siHijo; 															// Almacena la información de inicio del proceso hijo
																					//se va a utilizar para inicializar atributos de seguridad relacionados con la pipe que se creara
	SECURITY_ATTRIBUTES pipeSeg = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE}; 

	//Obtencion de información para la inicialización del proceso hijo
	GetStartupInfo(&siHijo);	
	//Cración de la tubería sin nombre
	CreatePipe(&hLecturaPipe, &hEscrituraPipe, &pipeSeg,0);
	//Escritura en la tuberia sin nombre
	WriteFile(hEscrituraPipe, matriz1,100*sizeof(int), &escritos, NULL);
	WriteFile(hEscrituraPipe, matriz2,100*sizeof(int), &escritos, NULL);
	printf("Procesoo HIJO creado.\n");
	printf("Matriz 1 y Matriz 2 enviadas al hijo para Multiplicar.\n");

	siHijo.hStdInput = hLecturaPipe; 												//se cambia la entrada estandar del hijo
	siHijo.hStdError = GetStdHandle (STD_ERROR_HANDLE);	
	siHijo.hStdOutput = GetStdHandle (STD_OUTPUT_HANDLE);
	siHijo.dwFlags = STARTF_USESTDHANDLES;											//indica que se usaran las banderas estandares cuando se cree el proceso hijo
	
	CreateProcess (NULL, "hijo.exe", NULL, NULL,
		TRUE, /* Hereda el proceso hijo los manejadores del padre */
		0, NULL, NULL, &siHijo, &piHijo);
	
	WaitForSingleObject (piHijo.hProcess, INFINITE);
	CloseHandle(hLecturaPipe);
	CloseHandle(hEscrituraPipe);
	CloseHandle(piHijo.hThread);
	CloseHandle(piHijo.hProcess);

	/***************Se saca la inversa de cada Matriz Resultado***********/
	/*********************************************************************/
	printf("\n 	PROCESO PADRE\n");
	int m_multiplicacion[FILAS][COLUMNAS];
	int m_suma[FILAS][COLUMNAS];

	leermatrizdearchivo(m_multiplicacion,0);
	leermatrizdearchivo(m_suma,1);

	printf("Matriz resultado de la Multiplicacion\n");
	imprimirmatrizres(m_multiplicacion);
	printf("Matriz resultado de la Suma\n");
	imprimirmatrizres(m_suma);

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

	return 0;
}
void llenarmatriz(int matriz[][COLUMNAS]){
	int i, j;
	for (i = 0; i < FILAS; i++){
		for(j = 0; j < COLUMNAS; j++){		
			matriz[i][j] = rand() % 10;
		}
	}
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
void leermatrizdearchivo(int matriz[][COLUMNAS], int n){
	if(n == 0){
		char cadena[10] = "";		
		int fila = 0;
		int columna = 0;
		FILE *archivo = fopen("MatrizMultiplicacion.txt", "r"); 							// Modo lectura
	    char bufer[LONGITUD_MAXIMA_CADENA];         										// Aquí vamos a ir almacenando cada línea
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
	if(n == 1){
		char cadena[10] = "";		
		int fila = 0;
		int columna = 0;
		FILE *archivo = fopen("MatrizSuma.txt", "r"); 											// Modo lectura
	    char bufer[LONGITUD_MAXIMA_CADENA];        												 // Aquí vamos a ir almacenando cada línea
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
//imprimir matrices resultado
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
void guardarmulti(float matrix[][COLUMNAS]){
	FILE *m1 = NULL;	
	m1 = fopen("MatrizMultiplicacion.txt","w+");
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
	m = fopen("MatrizSuma.txt","w+");
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

