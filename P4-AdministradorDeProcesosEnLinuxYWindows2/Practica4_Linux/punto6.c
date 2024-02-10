//Aplicacion que crea 5 hilos
//Hilo 1: Suma de dos matrices de 7x7 elementos de tipo entero
//Hilo 2: Resta sobre esas mismas matrices
//Hilo 3: Multiplicación de matrices
//Hilo 4: Obtener la transpuesta de cada matriz
//Hilo 5: Leera lo archivos de resultados y los mostrara en pantalla
//Los hilos del 1 -> 4 deberan crear un .txt con las salidas
//--------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
void hilo1();
void hilo2();
void hilo3();
void hilo4();
void hilo5();

	int m1[7][7] = {
		{1,2,3,4,5,6,7},
		{2,4,5,6,2,1,4},
		{7,4,2,6,3,1,3},
		{5,1,2,6,3,2,4},
		{1,3,2,5,4,7,6},
		{3,4,2,3,1,5,2},
		{7,6,5,4,3,2,1}
	};
	int m2[7][7] = {
		{7,4,2,6,3,1,3},
		{5,1,2,6,3,2,4},
		{1,2,3,4,5,6,7},
		{7,6,5,4,3,2,1},
		{3,4,2,3,1,5,2},
		{1,3,2,5,4,7,6},
		{2,4,5,6,2,1,4}
	};
	int mr[7][7];

int main(){	

	pthread_t th1,th2,th3,th4,th5; //Direcciones de los 5 hilos(threads)

	//Creacion de los hilos
	pthread_create(&th1,NULL,(void*)hilo1,NULL);//Hilo de la Suma
	pthread_join(th1,NULL);//Esperar a hilo1

	pthread_create(&th2,NULL,(void*)hilo2,NULL);//Hilo de la Resta
	pthread_join(th2,NULL);//Esperar a hilo2

	pthread_create(&th3,NULL,(void*)hilo3,NULL);//Hilo de la Multiplicación
	pthread_join(th3,NULL);//Esperar a hilo3

	pthread_create(&th4,NULL,(void*)hilo4,NULL);//Hilo de la Transpuesta
	pthread_join(th4,NULL);//Esperar a hilo4

	//Se crea el hilo 5 y se espera su finalización
	pthread_create(&th5,NULL,(void*)hilo5,NULL);//Hilo de los .txt
	pthread_join(th5,NULL);//Esperar a hilo5


}
void hilo1(){
	
	FILE *th1 = NULL;
	th1 = fopen("Hilo1.txt","w+");
	if(th1 == NULL){
		printf("No fue posible abrir el archivo\n");		
	}
	//Se imprime el Id. del Hilo 
	fprintf(th1,"Soy el primer Hilo: %lx\n",pthread_self());
	fprintf(th1,"Suma de matrices\n");

	for(int i = 0; i < 7; i++){
		//matriz 1
		for(int j = 0;j < 7; j++){	
			if(j == 0){
				fprintf(th1,"|   ");	
			}		
			fprintf(th1,"%d  ",m1[i][j]);
			if(j == 6){
				fprintf(th1,"  |");	
			}	
		}		
		if (i == 3){fprintf(th1," + ");}else{fprintf(th1,"   ");}		
		//matriz 2
		for(int j = 0;j < 7; j++){	
			if(j == 0){
				fprintf(th1,"|   ");	
			}		
			fprintf(th1,"%d  ",m2[i][j]);
			if(j == 6){
				if(i == 3){
					fprintf(th1,"  |  =\n");
				}
				else{
					fprintf(th1,"  |\n");	
				}
			}	
		}
	}
	fprintf(th1,"\n");
	for(int i = 0; i < 7; i++){
		for(int j = 0;j < 7; j++){
			if(j == 0){
				fprintf(th1,"|	");	
			}
			mr[i][j] = m1[i][j] + m2[i][j];
			fprintf(th1,"%d	",mr[i][j]);			
		}
		fprintf(th1,"|\n");							
	}
	fclose(th1);//Se cierra el archivo.txt	
	
	printf("Inicio Hilo 1\n");
	printf("Archivo creado\nFin Hilo 1\n");
	printf("\n");	
	pthread_exit(NULL); //Finaliza la ejecución del hilo
}

void hilo2(){
	
	FILE *th2 = NULL;
	th2 = fopen("Hilo2.txt","w+");
	if(th2 == NULL){
		printf("No fue posible abrir el archivo\n");		
	}
	//Se imprime el Id. del Hilo
	fprintf(th2,"Soy el segundo Hilo: %lx\n",pthread_self()); 
	fprintf(th2,"Resta de matrices\n");

	for(int i = 0; i < 7; i++){
		//matriz 1
		for(int j = 0;j < 7; j++){	
			if(j == 0){
				fprintf(th2,"|   ");	
			}		
			fprintf(th2,"%d  ",m1[i][j]);
			if(j == 6){
				fprintf(th2,"  |");	
			}	
		}		
		if (i == 3){fprintf(th2," - ");}else{fprintf(th2,"   ");}		
		//matriz 2
		for(int j = 0;j < 7; j++){	
			if(j == 0){
				fprintf(th2,"|   ");	
			}		
			fprintf(th2,"%d  ",m2[i][j]);
			if(j == 6){
				if(i == 3){
					fprintf(th2,"  |  =\n");
				}
				else{
					fprintf(th2,"  |\n");	
				}
			}	
		}
	}
	fprintf(th2,"\n");
	for(int i = 0; i < 7; i++){
		for(int j = 0;j < 7; j++){
			if(j == 0){
				fprintf(th2,"|	");	
			}
			mr[i][j] = m1[i][j] - m2[i][j];
			fprintf(th2,"%d	",mr[i][j]);			
		}
		fprintf(th2,"|\n");							
	}
	fclose(th2);//Se cierra el archivo.txt	
	
	printf("Inicio Hilo 2\n"); 
	printf("Archivo creado\nFin Hilo 2\n");	
	printf("\n");
	pthread_exit(NULL); //Finaliza la ejecución del hilo
}
void hilo3(){
	
	FILE *th3 = NULL;
	th3 = fopen("Hilo3.txt","w+");
	if(th3 == NULL){
		printf("No fue posible abrir el archivo\n");		
	}
	//Se imprime el Id. del Hilo
	fprintf(th3,"Soy el tercer Hilo: %lx\n",pthread_self()); 
	fprintf(th3,"Multiplicación de matrices\n");

	for(int i = 0; i < 7; i++){
		//matriz 1
		for(int j = 0;j < 7; j++){	
			if(j == 0){
				fprintf(th3,"|   ");	
			}		
			fprintf(th3,"%d  ",m1[i][j]);
			if(j == 6){
				fprintf(th3,"  |");	
			}	
		}		
		if (i == 3){fprintf(th3," * ");}else{fprintf(th3,"   ");}		
		//matriz 2
		for(int j = 0;j < 7; j++){	
			if(j == 0){
				fprintf(th3,"|   ");	
			}		
			fprintf(th3,"%d  ",m2[i][j]);
			if(j == 6){
				if(i == 3){
					fprintf(th3,"  |  =\n");
				}
				else{
					fprintf(th3,"  |\n");	
				}
			}	
		}
	}
	fprintf(th3,"\n");
	for(int a = 0;a < 7;a++){

		for(int i = 0; i < 7; i++){
			int suma = 0;	
			for(int j = 0;j < 7; j++){
				suma +=(m1[i][j] * m2[j][a]);
			}
			mr[i][a] = suma;			
		}		
	}
	for(int i = 0; i < 7; i++){
		for(int j = 0;j < 7; j++){
			if(j == 0){
				fprintf(th3,"|	");	
			}			
			fprintf(th3,"%d	",mr[i][j]);			
		}
		fprintf(th3,"|\n");							
	}
	fclose(th3);//Se cierra el archivo.txt	
	
	printf("Inicio Hilo 3\n");
	printf("Archivo creado\nFin Hilo 3\n");	
	printf("\n");
	pthread_exit(NULL); //Finaliza la ejecución del hilo
}
void hilo4(){

	FILE *th4 = NULL;
	th4 = fopen("Hilo4.txt","w+");
	if(th4 == NULL){
		printf("No fue posible abrir el archivo\n");		
	}
	//Se imprime el Id. del Hilo
	fprintf(th4,"Soy el cuarto Hilo: %lx\n",pthread_self()); 
	fprintf(th4,"Transpuesta de matrices\n");
	
	//Transpuesta de la matriz 1
	for(int i = 0; i < 7; i++){		
		for(int j = 0;j < 7; j++){	
			if(j == 0){
				fprintf(th4,"|   ");	
			}		
			fprintf(th4,"%d  ",m1[i][j]);
			if(j == 6){
				fprintf(th4,"  |");	
			}	
		}
		if (i == 0){fprintf(th4,"T  ");}
		else if (i == 3){fprintf(th4," = ");}
		else{fprintf(th4,"   ");}		
		//matriz 2
		for(int j = 0;j < 7; j++){	
			if(j == 0){
				fprintf(th4,"|   ");	
			}		
			fprintf(th4,"%d  ",m1[j][i]);
			if(j == 6){							
				fprintf(th4,"  |\n");					
			}	
		}
	}
	fprintf(th4,"\n");
	//Transpuesta de la matriz 2
	for(int i = 0; i < 7; i++){		
		for(int j = 0;j < 7; j++){	
			if(j == 0){
				fprintf(th4,"|   ");	
			}		
			fprintf(th4,"%d  ",m2[i][j]);
			if(j == 6){
				fprintf(th4,"  |");	
			}	
		}
		if (i == 0){fprintf(th4,"T  ");}
		else if (i == 3){fprintf(th4," = ");}
		else{fprintf(th4,"   ");}		
		//matriz 2
		for(int j = 0;j < 7; j++){	
			if(j == 0){
				fprintf(th4,"|   ");	
			}		
			fprintf(th4,"%d  ",m2[j][i]);
			if(j == 6){							
				fprintf(th4,"  |\n");					
			}	
		}
	}
	fprintf(th4,"\n");

	fclose(th4);//Se cierra el archivo.txt	
	
	printf("Inicio Hilo 4\n");
	printf("Archivo creado\nFin Hilo 4\n");	
	printf("\n");
	pthread_exit(NULL); //Finaliza la ejecución del hilo
}
void hilo5(){
	printf("Inicio Hilo 5\n");
	//Se imprime el Id. del Hilo
	printf("Soy el quinto Hilo: %lx\n",pthread_self()); 
	printf("Lectura de los archivos creados de los hilos anterires\n");

	char c;
	FILE *archivo1 = fopen("Hilo1.txt","r");
	if(archivo1!=NULL){
		printf("\n***Hilo1.txt***\n");
		while(1){
			c=fgetc(archivo1);
			if(feof(archivo1)){
				break;
			}		
			printf("%c",c);
		}
	}else{
		printf("Hubo un error al abrir el archivo o es inexistente.");
	}
	fclose(archivo1);

	FILE *archivo2 = fopen("Hilo2.txt","r");
	if(archivo2!=NULL){
		printf("\n***Hilo2.txt***\n");
		while(1){
			c=fgetc(archivo2);
			if(feof(archivo2)){
				break;
			}		
			printf("%c",c);
		}
	}else{
		printf("Hubo un error al abrir el archivo o es inexistente.");
	}
	fclose(archivo2);

	FILE *archivo3 = fopen("Hilo3.txt","r");
	if(archivo3!=NULL){
		printf("\n***Hilo3.txt***\n");
		while(1){
			c=fgetc(archivo3);
			if(feof(archivo3)){
				break;
			}		
			printf("%c",c);
		}
	}else{
		printf("Hubo un error al abrir el archivo o es inexistente.");
	}
	fclose(archivo3);

	FILE *archivo4 = fopen("Hilo4.txt","r");
	if(archivo4!=NULL){
		printf("\n***Hilo4.txt***\n");
		while(1){
			c=fgetc(archivo4);
			if(feof(archivo4)){
				break;
			}		
			printf("%c",c);
		}
	}else{
		printf("Hubo un error al abrir el archivo o es inexistente.");
	}
	fclose(archivo4);

	printf("Fin Hilo 5\n");
	pthread_exit(NULL); //Finaliza la ejecución del hilo
}

