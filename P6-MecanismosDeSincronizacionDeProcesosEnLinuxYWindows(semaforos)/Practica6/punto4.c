/*
	Programe la misma aplicación del punto 7 de la práctica 5(tanto para Linux como para Windows), 
	utilizando como máximo tres regiones de memoria compartida de 400 bytes cada una para almacenar 
	todas las matrices requeridas por la aplicación. Utilice como mecanismo de sincronización los 
	semáforos revisados en esta práctica tanto para la escritura como para la lectura de las memorias
	compartidas. Úselos en los lugares donde haya necesidad de sincronizar el acceso a memoria compartida.
	

	-> 7.- PRACTICA 5
	Programe nuevamente la aplicación del punto cuatro utilizando en esta ocasión memoria compartida en
	lugar de tuberías (utilice tantas memorias compartidas como requiera). Programe esta aplicación para 
	Linux como para Windows utilizando la memoria compartida de cada sistema operativo.
	
	-> 4.- PRACTICA 5
	Programe una aplicación que cree un proceso hijo a partir de un proceso padre, el proceso padre enviará
	al proceso hijo, a travéz de una tuberia, dos matrices de 10 x 10 a multiplicar por parte del hijo, 
	mientras tanto el proceso hijo creará un hijo de él, al cual enviará dos matrices de 10 x 10 a sumar en
	el proceso hijo creado, nuevamente el envío de estos valores será a través de una tubería. Una vez 
	calculando el resultado de la suma, el proceso hijo del hijo devolverá la matriz resultante a su abuelo 
	(via tuberiá). A su vez, el proceso hijo devolverá la matriz resultante de la multiplicación que realizó 
	a su padre. Finalmente, el proceso padre obtendra la matriz inversa de cada una de las matrices recibidas
	y el resultado lo guardará en un archivo para cada matriz inversa obtenida.
	
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
				-Regresar la matriz resultante e la suma al ABUELO 
*/
//pid == 0  <- Proceso Hijo
//pid < 0  <- Proceso Padre
//pid == -1 <-Error>

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <time.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>


const int FILAS = 10;
const int COLUMNAS = 10;
const int TAM_NMEM = 120*sizeof(int);

//memoria compartida para matrices
void llenarmatriz(int matriz[][COLUMNAS]);
void imprimirmatriz(int matriz[][COLUMNAS]);
void matrizatexto(int matriz[][COLUMNAS], char* cad);
void cadenaAentero(char* cad,int matriz[][COLUMNAS]);
void concatenar(char* cad, char caracter);
//operaciones con matrices
void imprimirmatrizres(int m[][COLUMNAS]);
void imprimirmatriz_float(float m[][COLUMNAS]);
void imprimirmatrizres_float(float m[][COLUMNAS]);
void multiplicacion(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]);
void suma(int m1[][COLUMNAS],int m2[][COLUMNAS],int mr[][COLUMNAS]);
void copiarmatriz(int m1[][COLUMNAS],float mr[][COLUMNAS]);
void inversa(float matrix[][COLUMNAS]);
void guardarmulti(int matrix[][COLUMNAS]);
void guardarmultiinversa(float matrix[][COLUMNAS]);
void guardarsuma(int matrix[][COLUMNAS]);
void guardarsumainversa(float matrix[][COLUMNAS]);
void leermultiplicacion(int matrix[][COLUMNAS]);
//void leersuma(int matrix[][COLUMNAS]);
void cadenaAmatriz(char* cad,int matriz[][COLUMNAS]);
void leersuma(int matrix[][COLUMNAS]);

int main(void){
	srand(time(NULL));
	
	int i,j;
	pid_t pid;
	int semid; //Se usa para guardar el identificador del sistema operativo va a devolver cuando se crea el semaforo
	key_t llave = 1234; //Similar a memoria compartida, para un semaforo se requiere una variable que identifique al semaforo con el que se esta trabajando
	int semban = IPC_CREAT | 0666; //Esta variable va a almacenar las banderas para la creación del semaforo
	int nsems = 1; //Esta variable nos indica cuantos semaforos vamos a crear en el grupo
	int nsops; //Nos indica el numero de estructuras que vamos a utilizar con una operación del semaforo
	struct sembuf *sops = (struct sembuf *) malloc(2*sizeof(struct sembuf)); //sops  es del tipo struct sembuf, para detallar que es lo que debe de hacer la operación del semaforo a utilizar
	//Comienza la ejecución del programa
	printf("Iniciando semaforo...\n");
	//semget(llave del semaforo, numero de semaforos, banderas para la creación del semaforo)	
	if ((semid = semget(llave, nsems, semban)) == -1) { //usando semget se crea el grupo de semaforos a usar
		perror("semget: error al iniciar semaforo");
		exit(1);
	}
	else
		printf("Semaforo iniciado...\n");
	if ((pid = fork()) < 0) {
		perror("fork: error al crear proceso\n");
		exit(1);
	}
	if (pid == 0) {
		//PROCESO HIJO
		i = 0;
		while (i < 1) { //Las 3 veces que el proceso va a entrar a la seccion critica
			nsops = 2; //numero de estructuras de tipo sembuf con las que se trabajaran las operaciones con el semaforo
			// la estructura sops tiene tres mienbros
			// sen_num, sem_op, sem_flg
			sops[0].sem_num = 0; //numero del semaforo dentro del grupo sobre el cual se va a aplicar esta estructura sops
			sops[0].sem_op = 0; //nos indica como vamos a trabajar la operación del semaforo que se manipulara con la estructura sem
			sops[0].sem_flg = SEM_UNDO; //las banderas para la estructura en las operaciones
			//SEM_UNDO se utiliza para indicar que la estructura dentro de la operacion se debe de llevar de manera atomica(que si llega a haber algun problema se va a desaser lo que se realizo en el semafor,
			//regresando al estado estable del semaforo)
			sops[1].sem_num = 0;
			sops[1].sem_op = 1; //es para cambiar el valor del semaforo cuando el semaforo esta ocupado
			sops[1].sem_flg = SEM_UNDO | IPC_NOWAIT;

			if ((j = semop(semid, sops, nsops)) == -1) {
				perror("semop: error en operacion del semaforo\n");
			}
			else {				
				printf("\n\nProceso hijo toma el control del semaforo.\n");
				printf("Soy el hijo(%d, hijo de %d)\n",getpid(), getppid());				
				printf("El hijo esta en la sección critica\n");
				{
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
					sM1 = shmM1;
					strcpy(cadena1,sM1);
					//printf("String: \n%s\n\n",sM1);
					//printf("Cadena: \n%s\n\n",cadena);
					//for(sM1 = shmM1; *sM1 != '\0';sM1++)
					//	putchar(*sM1);		

					cadenaAentero(cadena1,matriz1);
					printf("Matriz 1 recibida:\n");
					imprimirmatriz(matriz1);					
					//*shmM1 = '*';	
					
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
					//printf("String2: \n%s\n\n",sM2);

					cadenaAentero(cadena2,matriz2);
					printf("Matriz 2 recibida:\n");
					imprimirmatriz(matriz2);

					int mr[FILAS][COLUMNAS];
					multiplicacion(matriz1,matriz2,mr); //Se hace la multiplicacion
					guardarmulti(mr);
					printf("Multiplicación:\n");
					imprimirmatriz(mr);					
					//*shmM2 = '*';
					//exit(0);				
				}
				 // Aqui es para simular lo que el proceso hijo estaria ejecutando como parte de su seccion critica, como lo es acceso a memoria compartida, acceso a un archivo compartido
				//un recurso compartido, etc.
				//Antes de salir el proceso debe de regresar al semaforo a su estado de desocupado:
				nsops = 1;
				sops[0].sem_num = 0;
				sops[0].sem_op = -1;
				sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT;
				if ((j = semop(semid, sops, nsops)) == -1) {
					perror("semop: error en operacion del semaforo\n");
				}
				else					
					printf("Proceso hijo regresa el control del semaforo.\n");
					printf("\n");
					printf("\n");
					{
						pid_t pid2;
						if((pid2 = fork()) == 0){
							//PROCESO NIETO	
							printf("Soy el nieto(%d, hijo de %d)\n",getpid(), getppid());							
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
							sM1 = shmM1;
							strcpy(cadena1,sM1);
							//printf("String: \n%s\n\n",sM1);
							//printf("Cadena: \n%s\n\n",cadena);
							//for(sM1 = shmM1; *sM1 != '\0';sM1++)
							//	putchar(*sM1);		

							cadenaAentero(cadena1,matriz1);
							printf("Matriz 1 recibida:\n");
							imprimirmatriz(matriz1);					
							//*shmM1 = '*';	
							
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
							//printf("String2: \n%s\n\n",sM2);

							cadenaAentero(cadena2,matriz2);
							printf("Matriz 2 recibida:\n");
							imprimirmatriz(matriz2);

							int mr[FILAS][COLUMNAS];
							suma(matriz1,matriz2,mr); //Se hace la suma
							guardarsuma(mr);
							printf("Suma:\n");
							imprimirmatriz(mr);	
							printf("\n");
							printf("\n");

						}
						
					}
				sleep(5);
			}
			++i;
		}
	}
	//PROCESO PADRE
	else {
		i = 0;
		while (i < 1) {
			nsops = 2;
			sops[0].sem_num = 0;
			sops[0].sem_op = 0;
			sops[0].sem_flg = SEM_UNDO;

			sops[1].sem_num = 0;
			sops[1].sem_op = 1;
			sops[1].sem_flg = SEM_UNDO | IPC_NOWAIT;
		
			if ((j = semop(semid, sops, nsops)) == -1) {
				perror("semop: error en operacion del semaforo\n");
			}else {				
				printf("Proceso padre toma el control del semaforo.\n");
				printf("Soy el padre(%d, hijo de %d)\n",getpid(), getppid());				
				printf("El padre esta en la sección critica\n");
				{
					//Matriz 1 

					int matriz1[FILAS][COLUMNAS];
					llenarmatriz(matriz1);
					printf("Matriz 1 generada:\n");
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
					//for(sM1 = shmM1; *sM1 != '\0';sM1++)
					//	putchar(*sM1);		
					//putchar('\n');
						
					//while(*shmM1!='*')
						//sleep(1); //El cliente es el que escribe el a
					//exit(0);

					//Matriz 2

					int matriz2[FILAS][COLUMNAS];
					llenarmatriz(matriz2);
					printf("Matriz 2 generada:\n");
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

					//for(sM2 = shmM2; *sM2 != '\0';sM2++)
					//	putchar(*sM2);		
					//putchar('\n');
						
					//while(*shmM2!='*')
					//	sleep(1); //El cliente es el que escribe el a
					//exit(0);
				}
				nsops = 1;
				sops[0].sem_num = 0;
				sops[0].sem_op = -1;
				sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT;
				if ((j = semop(semid, sops, nsops)) == -1) {
					perror("semop: error en semop()\n");
				}
				else
					printf("Proceso padre regresa el control del semaforo.\n");
				sleep(5);
			}
			++i;
		}
		printf("PROCESO PADRE\n");
		printf("Matriz resultante de la Multiplicacióń:\n");
		int m_multiplicacion[FILAS][COLUMNAS];
		leermultiplicacion(m_multiplicacion);
		imprimirmatriz(m_multiplicacion);
		printf("\n");
		printf("Matriz resultante de la Suma:\n");
		int m_suma[FILAS][COLUMNAS];
		leersuma(m_suma);
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
		
		guardarmultiinversa(m_inversa_multi);
		guardarsumainversa(m_inversa_suma);

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

    A=(float **)malloc(matsize*sizeof(float *));            //allocate memory dynamically for matrix A(matsize X matsize)
	
    for(i=0;i<matsize;i++)
        A[i]=(float *)malloc(matsize*sizeof(float));

    I=(float **)malloc(matsize*sizeof(float *));            //memory allocation for indentity matrix I(matsize X matsize)

    for(i=0;i<matsize;i++)
        I[i]=(float *)malloc(matsize*sizeof(float));

    // se cargan los elementos de la matriz a A
    for(i=0;i<matsize;i++)
        for(j=0;j<matsize;j++)
        	A[i][j] = matrix[i][j];            


    for(i=0;i<matsize;i++)                                  //automatically initialize the unit matrix, e.g.
        for(j=0;j<matsize;j++)                              //  -       -
            if(i==j)                                        // | 1  0  0 |
                I[i][j]=1;                                  // | 0  1  0 |
            else                                            // | 0  0  1 |
                I[i][j]=0;                                  //  -       -
/*---------------LoGiC starts here------------------*/      //procedure // to make the matrix A to unit matrix

    for(k=0;k<matsize;k++)                                  //by some row operations,and the same row operations of
    {                                                       //Unit mat. I gives the inverse of matrix A
        temp=A[k][k];                  					 	//'temp'  
        													// stores the A[k][k] value so that A[k][k]  will not change
        for(j=0;j<matsize;j++)     							//during the operation //A[i] //[j]/=A[k][k]  when i=j=k
        {
            A[k][j]/=temp;                                  //it performs // the following row operations to make A to unit matrix
            I[k][j]/=temp;                                  //R0=R0/A[0][0],similarly for I also R0=R0/A[0][0]
        }                                                   //R1=R1-R0*A[1][0] similarly for I
        for(i=0;i<matsize;i++)                              //R2=R2-R0*A[2][0]      ,,
        {
            temp=A[i][k];                       //R1=R1/A[1][1]
            for(j=0;j<matsize;j++)             //R0=R0-R1*A[0][1]
            {                                   //R2=R2-R1*A[2][1]
                if(i==k)
                    break;                      //R2=R2/A[2][2]
                A[i][j]-=A[k][j]*temp;          //R0=R0-R2*A[0][2]
                I[i][j]-=I[k][j]*temp;          //R1=R1-R2*A[1][2]
            }
        }
    }
/*---------------LoGiC ends here--------------------*/
    //printf("The inverse of the matrix is: ");               //Print the //matrix I that now contains the inverse of mat. A
    for(i=0;i<matsize;i++)
    {
        for(j=0;j<matsize;j++)
        	matrix[i][j] = I[i][j];            
    }
    
}
void guardarmulti(int matrix[][COLUMNAS]){
	FILE *m1 = NULL;	
	m1 = fopen("Multiplicación.txt","w+");
	if(m1 == NULL){
		printf("No fue posible abrir el archivo\n");
	}	
	for (int i = 0; i < FILAS; i++){
		fprintf(m1,"<");	
		for(int j = 0; j < COLUMNAS; j++){								
			fprintf(m1,"%d|",matrix[i][j]);																							
		}
		fprintf(m1,">\n");	
	}
	fclose(m1);//Se cierra el archivo
}
void guardarsuma(int matrix[][COLUMNAS]){
	FILE *m = NULL;	
	m = fopen("Suma.txt","w+");
	if(m == NULL){
		printf("No fue posible abrir el archivo\n");
	}	
	for (int i = 0; i < FILAS; i++){
		fprintf(m,"<");	
		for(int j = 0; j < COLUMNAS; j++){			
			fprintf(m,"%d|",matrix[i][j]);			
		}
		fprintf(m,">\n");	
	}
	fclose(m);//Se cierra el archivo
}
void guardarmultiinversa(float matrix[][COLUMNAS]){
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
void guardarsumainversa(float matrix[][COLUMNAS]){
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
void leermultiplicacion(int matrix[][COLUMNAS]){
	const char* archivo = "Multiplicación.txt";

	FILE* input_file = fopen(archivo, "r");
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
void leersuma(int matrix[][COLUMNAS]){
	const char* archivo = "Suma.txt";

	FILE* input_file = fopen(archivo, "r");
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
