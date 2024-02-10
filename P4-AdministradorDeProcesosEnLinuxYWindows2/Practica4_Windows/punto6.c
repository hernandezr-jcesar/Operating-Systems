//Aplicacion que crea 5 hilos
//Hilo 1: Suma de dos matrices de 7x7 elementos de tipo entero
//Hilo 2: Resta sobre esas mismas matrices
//Hilo 3: Multiplicación de matrices
//Hilo 4: Obtener la transpuesta de cada matriz
//Hilo 5: Leera lo archivos de resultados y los mostrara en pantalla
//Los hilos del 1 -> 4 deberan crear un .txt con las salidas
//--------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

DWORD WINAPI hilo1();
DWORD WINAPI hilo2();
DWORD WINAPI hilo3();
DWORD WINAPI hilo4();
DWORD WINAPI hilo5();

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
	DWORD idHilo1;
	HANDLE manHilo1;
	
	DWORD idHilo2;
	HANDLE manHilo2;
	
	DWORD idHilo3;
	HANDLE manHilo3;
	
	DWORD idHilo4;
	HANDLE manHilo4;
	
	DWORD idHilo5;
	HANDLE manHilo5;
	
	//Creacion de hilo 1
	manHilo1=CreateThread(NULL,0,hilo1,NULL,0,&idHilo1);
	//Esperar la finanilación del hilo 1
	WaitForSingleObject(manHilo1,INFINITE);	
	//Cierre del manejador del hilo 1 creado
	CloseHandle(manHilo1);
	
	//Creacion de hilo 2
	manHilo2=CreateThread(NULL,0,hilo2,NULL,0,&idHilo2);
	//Esperar la finanilación del hilo 2
	WaitForSingleObject(manHilo2,INFINITE);	
	//Cierre del manejador del hilo 2 creado
	CloseHandle(manHilo2);
	
	//Creacion de hilo 3
	manHilo3=CreateThread(NULL,0,hilo3,NULL,0,&idHilo3);
	//Esperar la finanilación del hilo 3
	WaitForSingleObject(manHilo3,INFINITE);	
	//Cierre del manejador del hilo 3 creado
	CloseHandle(manHilo3);
	
	//Creacion de hilo 4
	manHilo4=CreateThread(NULL,0,hilo4,NULL,0,&idHilo4);
	//Esperar la finanilación del hilo 4
	WaitForSingleObject(manHilo4,INFINITE);	
	//Cierre del manejador del hilo 4 creado
	CloseHandle(manHilo4);
	
	//Creacion de hilo 5
	manHilo5=CreateThread(NULL,0,hilo5,NULL,0,&idHilo5);
	//Esperar la finanilación del hilo 5
	WaitForSingleObject(manHilo5,INFINITE);	
	//Cierre del manejador del hilo 5 creado
	CloseHandle(manHilo5);
	
	return 0;
}
DWORD WINAPI hilo1(){
	printf("Inicio Hilo 1\n");
	
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

	printf("Archivo creado\nFin Hilo 1\n");
	printf("\n");	
	
	return 0;
}

DWORD WINAPI hilo2(){
	printf("Inicio Hilo 2\n"); 
	
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
	
	printf("Archivo creado\nFin Hilo 2\n");	
	printf("\n");
	return 0;
}

DWORD WINAPI hilo3(){
	printf("Inicio Hilo 3\n");
	
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
		
	printf("Archivo creado\nFin Hilo 3\n");	
	printf("\n");
	return 0;
}
DWORD WINAPI hilo4(){
	printf("Inicio Hilo 4\n");
	
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
		
	printf("Archivo creado\nFin Hilo 4\n");	
	printf("\n");
	return 0;
}
DWORD WINAPI hilo5(){
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
	return 0;
}

