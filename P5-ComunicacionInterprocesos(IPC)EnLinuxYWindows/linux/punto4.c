/*
	Programa que crea un proceso hijo a partir de un proceso padre,
	el proceso padre enviará al proceso hij, a través de una tuberia,
	dos matrices de 10x10 a multiplicar por parte del hijo, mientras
	tanto el proceso hijo creará un hijo de él, al cual enviara dos 
	matrices de 10x10 a sumar en el proceso hijo creado, nuevamente 
	el envío de estos valores será a través de una tuberia. 
	Una vez calculado el resultado de la suma, el proceso hijo del hijo
	devolvera la matriz resultante a su abuelo(via tuberia).A su vez
	el proceso hijo devolvera la matriz resultante de la multiplicación 
	que realizó a su padre. 
	Finalmente, el proceso padre obtendrá la matriz inversa 
	de cada una de las matrices recibidas y el resultado lo guardará en 
	un archivo para cada matriz inversa obtenida.

	RESUMEN
	Proceso PADRE:
		-crear un proceso HIJO
		-enviar al HIJO dos matrices 10x10 para que se multipliquen
				-Recibir la matriz resultante de la multiplicación
				-Recibir la matriz resultante de la suma
					-Obtener la matriz inversa de la matriz recibida del NIETO
						-Guardar el resultado en un archivo de texto
					-Obtener la matriz inversa de la matriz recibida del HIJO
						-Guardar el resultado en un archivo de texto
	Proceso HIJO:
		-Recibe dos matrices 10x10 de su PADRE y las multiplica
		-Crear un proceso HIJO
			-Enviar al HIJO dos matrices 10x10 para que se sumen
				-Regresar la matriz resulatante de la multiplicacion a su PADRE
	Proceso HIJO DEL HIJO:
			-Recibe dos matrices 10x10 de su PADRE y las suma
				-Regresar la matriz resultante de la suma al ABUELO 
*/
//pid == 0  <- Proceso Hijo
//pid < 0  <- Proceso Padre
//pid == -1 <-Error


//pipe [0] -> Lectura / read
//pipe [1] -> Escritura / write
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

const int FILAS = 10;
const int COLUMNAS = 10;

void llenarmatriz(int m[][COLUMNAS]);
void imprimirmatriz(int m[][COLUMNAS]);
void imprimirmatrizres(int m[][COLUMNAS]);
void imprimirmatriz_float(float m[][COLUMNAS]);
void imprimirmatrizres_float(float m[][COLUMNAS]);
void multiplicacion(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]);
void suma(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]);
void copiarmatriz(int m1[][COLUMNAS],float mr[][COLUMNAS]);
void inversa(float matrix[][COLUMNAS]);
void guardarmulti(float matrix[][COLUMNAS]);
void guardarsuma(float matrix[][COLUMNAS]);

int main(int argc, char const *argv[])
{
	//int matriz[FILAS][COLUMNAS];
	//llenarmatriz(matriz);
	//imprimirmatriz(matriz);
	srand(time(NULL));

	int pipePIw[2];// PADRE-HIJO-write
	if(pipe(pipePIw) == -1){
		perror("pipe");
		exit(1);
	}

	int pipeIPw[2];// HIJO-PADRE-write
	if(pipe(pipeIPw) == -1){
		perror("pipe");
		exit(1);
	}

	int pipeNPw[2];// NIETO-PADRE-write
	if(pipe(pipeNPw) == -1){
		perror("pipe");
		exit(1);
	}


	pid_t pid;
	pid = fork();
	if(pid == -1){
		return 1;
	}

	if(pid == 0){ //Estamos en el HIJO
		wait(NULL);
		printf("Soy el proceso HIJO(%d), hijo de(%d).\n",getpid(),getppid());

		
		close(pipePIw[1]);
		int matriz1[FILAS][COLUMNAS];
		int matriz2[FILAS][COLUMNAS];

		read(pipePIw[0],matriz1,100*sizeof(int));
		read(pipePIw[0],matriz2,100*sizeof(int));
		printf("Matriz 1:\n");
		imprimirmatriz(matriz1); //impresion m1
		printf("\n");
		printf("Matriz 2:\n");
		imprimirmatriz(matriz2); //impresion m2
		printf("\n");
		int mr[FILAS][COLUMNAS];
		multiplicacion(matriz1,matriz2,mr); //Se hace la multiplicacion
		printf("Multiplicación:\n");
		imprimirmatrizres(mr); //Se imprime la matriz resultado

		close(pipePIw[0]);
		
			int pipeINw[2];// HIJO-NIETO-write

			if(pipe(pipeINw) == -1){
				perror("pipe");
				exit(1);
			}

			pid_t id;
			id = fork();
			if (id == 0) //Estamos en el NIETO
			{
				wait(NULL);
				printf("Soy el proceso NIETO(%d), hijo de(%d).\n",getpid(),getppid());

				close(pipeINw[1]);
				int matriz1[FILAS][COLUMNAS];
				int matriz2[FILAS][COLUMNAS];

				read(pipeINw[0],matriz1,100*sizeof(int));
				read(pipeINw[0],matriz2,100*sizeof(int));
				printf("Matriz 1:\n");
				imprimirmatriz(matriz1); //impresion m1
				printf("\n");
				printf("Matriz 2:\n");
				imprimirmatriz(matriz2); //impresion m2
				printf("\n");
				int mr[FILAS][COLUMNAS];
				suma(matriz1,matriz2,mr); //Se hace la multiplicacion
				printf("Suma:\n");
				imprimirmatrizres(mr); //Se imprime la matriz resultado
				close(pipeINw[0]);

				close(pipeNPw[0]);
				write(pipeNPw[1], mr,100*sizeof(int));
				printf("Matriz resultante de la suma enviada.\n\n");	
				close(pipeNPw[1]);
			}else{
				close(pipeINw[0]);
				int matriz1[FILAS][COLUMNAS];
				llenarmatriz(matriz1);

				int matriz2[FILAS][COLUMNAS];
				llenarmatriz(matriz2);

				write(pipeINw[1], matriz1,100*sizeof(int));
				printf("Matriz 1 enviada a hijo para suma.\n\n");	

				write(pipeINw[1], matriz2,100*sizeof(int));
				printf("Matriz 2 enviada a hijo para suma.\n\n");

				close(pipeINw[1]);

			}

		close(pipeIPw[0]);

		write(pipeIPw[1], mr,100*sizeof(int));
		printf("Matriz resultante de la multiplicacion enviada.\n\n");	

		close(pipeIPw[1]);
			

	}else{ //Estamos en el PADRE
		
		printf("Soy el proceso PADRE(%d)\n",getpid());
		

		close(pipePIw[0]);
		int matriz1[FILAS][COLUMNAS];
		int matriz2[FILAS][COLUMNAS];

		llenarmatriz(matriz1);
		llenarmatriz(matriz2);

		//le manda m1 a HIJO
		write(pipePIw[1], matriz1,100*sizeof(int));
		printf("Matriz 1 enviada para multiplicar.\n");	
		//le manda m2 a HIJO
		write(pipePIw[1], matriz2,100*sizeof(int));
		printf("Matriz 2 enviada para multiplicar\n");

		close(pipePIw[1]);
		//esperamos a que los hijos terminen
		wait(NULL);
		//Matriz resultante de la multiplicacion
		close(pipeIPw[1]);
		int m_multiplicacion[FILAS][COLUMNAS];
		read(pipeIPw[0],m_multiplicacion,100*sizeof(int));
		printf("Matriz resultate de la multiplicacion:\n");
		imprimirmatrizres(m_multiplicacion);
		close(pipeIPw[0]);

		//Matriz resultante de la suma
		close(pipeNPw[1]);
		int m_suma[FILAS][COLUMNAS];
		read(pipeNPw[0],m_suma,100*sizeof(int));
		printf("Matriz resultate de la suma:\n");
		imprimirmatrizres(m_suma);
		close(pipeNPw[0]);

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
//imprimir matrices resultado
void imprimirmatrizres(int matriz[][COLUMNAS]){
	for (int i = 0; i < FILAS; i++){
		for(int j = 0; j < COLUMNAS; j++){
			if(j == 0){
				//printf("|   ");	
			}
			printf("%d	",matriz[i][j]);
			if(j == 9){
				printf("  \n");	
			}		
		}
	}
}
void imprimirmatriz_float(float matriz[][COLUMNAS]){
	for (int i = 0; i < FILAS; i++){
		for(int j = 0; j < COLUMNAS; j++){			
			printf("%0.1f ",matriz[i][j]);
			if(j == 9){
				printf("  \n");	
			}		
		}
	}
}
//imprimir matrices resultado
void imprimirmatrizres_float(float matriz[][COLUMNAS]){
	for (int i = 0; i < FILAS; i++){
		for(int j = 0; j < COLUMNAS; j++){			
			printf("%0.4f	",matriz[i][j]);
			if(j == 9){
				printf("  \n");	
			}		
		}
	}
}
void multiplicacion(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]){
	for(int a = 0;a < FILAS;a++){
		for(int i = 0; i < FILAS; i++){
			int suma = 0;	
			for(int j = 0;j < FILAS; j++){
				suma +=(m1[i][j] * m2[j][a]);
			}
			mr[i][a] = suma;			
		}		
	}
}
void suma(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]){
	for(int i = 0; i < FILAS; i++){
		for(int j = 0;j < FILAS; j++){		
			mr[i][j] = m1[i][j] + m2[i][j];				
		}
	}
}

void copiarmatriz(int m1[][COLUMNAS],float mr[][COLUMNAS]){
	for (int i = 0; i < FILAS; i++){
		for(int j = 0; j < COLUMNAS; j++){	
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
	m1 = fopen("MatrizInversaMultiplicación.txt","w+");
	if(m1 == NULL){
		printf("No fue posible abrir el archivo\n");
	}
	fprintf(m1,"Matriz inversa de la Multiplicacióń:\n");
	for (int i = 0; i < FILAS; i++){
		for(int j = 0; j < COLUMNAS; j++){			
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
	for (int i = 0; i < FILAS; i++){
		for(int j = 0; j < COLUMNAS; j++){			
			fprintf(m,"%0.4f	",matrix[i][j]);
			if(j == 9){
				fprintf(m,"	\n");	
			}		
		}
	}
	fclose(m);//Se cierra el archivo
}

