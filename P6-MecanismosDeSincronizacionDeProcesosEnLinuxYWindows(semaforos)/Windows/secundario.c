#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#define TAM_MEM 400

int main(int argc, char *argv[]){
	STARTUPINFO siH;
	PROCESS_INFORMATION piH;
	ZeroMemory(&siH, sizeof(siH));
	siH.cb = sizeof(siH);
	ZeroMemory(&piH, sizeof(piH));
	HANDLE hSemaforo;
	
	int A[10][10], B[10][10], b[10][10], c[10][10], d[10][10], mandada1[10][10], mandada2[10][10], producto[10][10];
	int aux, suma;
	char *PH = "PadreHijo"; //padre hijo
	char *HP = "HijoPadre"; //hijo padre
	char *HN = "HijoNieto"; //hijo nieto
	HANDLE hArchMapeoPH, hArchMapeoHP, hArchMapeoHN;
	int i, j, k, shmid,ct=0;
	int *aPH, *aHP, *aHN;
	srand(time(NULL));
	int *shmPH, *shmHP, *shmHN;
	
	if((hSemaforo=OpenSemaphore(SEMAPHORE_ALL_ACCESS,TRUE,"Semaforo"))==NULL){
		printf("Falla al invocar OpenSemaphore:%d\n",GetLastError());
		return -1;
	}
		
	if (!CreateProcess(NULL,"prueba3", NULL, NULL, FALSE, 0, NULL, NULL, &siH, &piH)){
		printf("Fallo al invocar CreateProcess(%d)\n", GetLastError());
		return -1;
	}
	puts("Se creo el proceso hijo(en el hijo)\n");	
	
	//MANDA MATRIZ A NIETO
	if ((hArchMapeoHN = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, TAM_MEM, HN)) == NULL){
		printf("No se mapeo la memoria compartida: (%i)\n", GetLastError());
		exit(-1);
	}
	puts("Se creo el mapeo hijo");
	if ((shmHN = (int *)MapViewOfFile(hArchMapeoHN, FILE_MAP_ALL_ACCESS, 0, 0, TAM_MEM)) == NULL){
		printf("No se creo la memoria compartida: (%i)\n", GetLastError());
		CloseHandle(hArchMapeoHN);
		exit(-1);
	}
	puts("Se creo la memoria com hijo\n");
	aHN = shmHN;

	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			*aHN = rand() % 15 * 7;
			mandada1[i][j] = *aHN;
			*aHN++;
		}
	}
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			*aHN = rand() % 15 * 8;
			mandada2[i][j] = *aHN;
			*aHN++;
		}
	}
	*aHN = 101;
	while (*shmHN != -1){
		printf("Esperando Hijo->Nieto");
//		sleep(1);
	}
	WaitForSingleObject(hSemaforo,INFINITE);
	printf("2 MATRICES. HIJO -> NIETO. HIJO.\nMatriz 1.\n");
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			printf("%d\t", mandada1[i][j]);
		}
		printf("\n");
	}
	printf("Matriz 2.\n");
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			printf("%d\t", mandada2[i][j]);
		}
		printf("\n");
	}
	UnmapViewOfFile(shmHN);
	CloseHandle(hArchMapeoHN);
	
	if(!ReleaseSemaphore(hSemaforo,1,NULL)){
		printf("Falla al invocar ReleaseSemaphore: %d\n",GetLastError());
	}

	if((hSemaforo=OpenSemaphore(SEMAPHORE_ALL_ACCESS,TRUE,"Semaforo"))==NULL){
		printf("Falla al invocar OpenSemaphore:%d\n",GetLastError());
		return -1;
	}

	//RECIBE MATRIZ DEL PADRE
	if ((hArchMapeoPH = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, PH)) == NULL){
		printf("No se ario archsadfadsfdfdfdivo de mapeo de la memoria: (%i)\n", GetLastError());
		exit(-1);
	}
	if ((shmPH = (int *)MapViewOfFile(hArchMapeoPH, FILE_MAP_ALL_ACCESS, 0, 0, TAM_MEM)) == NULL){
		printf("No se accedio a la memoria compartida: (%i)\n", GetLastError());
		CloseHandle(hArchMapeoPH);
		exit(-1);
	}
	aPH = shmPH;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			A[i][j] = *aPH;
			aPH++;
		}
	}
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			B[i][j] = *aPH;
			aPH++;
		}
	}
	*shmPH = -1;
	printf("2 MATRICES. PADRE -> HIJO. HIJO.\nMatriz 1\n");
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			printf("%d\t", A[i][j]);
		}
		printf("\n");
	}
	printf("\nMatriz 2\n");
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			printf("%d\t", B[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	UnmapViewOfFile(shmPH);
	CloseHandle(hArchMapeoPH);
	//HACE EL PRODUCTO
	for (i = 0; i<10; i++){
		for (j = 0; j<10; j++){
			aux = 0;
			suma = 0;
			while (aux<10){
				suma += A[i][aux] * B[aux][j];
				aux++;
			}
			producto[i][j] = suma;
		}
	}
	
	//MANDA MATRIZ AL PADRE
	if ((hArchMapeoHP = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, TAM_MEM, HP)) == NULL){
		printf("No se mapeo la memoria compartida: (%i)\n", GetLastError());
		exit(-1);
	}
	if ((shmHP = (int *)MapViewOfFile(hArchMapeoHP, FILE_MAP_ALL_ACCESS, 0, 0, TAM_MEM)) == NULL){
		printf("No se creo la memoria compartida: (%i)\n", GetLastError());
		CloseHandle(hArchMapeoHP);
		exit(-1);
	}
	aHP = shmHP;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){	
			*aHP = producto[i][j];
			*aHP++;
		}
	}
	printf("PRODUCTO. HIJO -> PADRE. HIJO.\n");
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			printf("%d\t", producto[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	*aHP = 101;
	while (*shmHP != -1){
		puts("Esperando Hijo->Padre");
//		sleep(1);
	}
	WaitForSingleObject(hSemaforo,INFINITE);	
	if(!ReleaseSemaphore(hSemaforo,1,NULL)){
		printf("Falla al invocar ReleaseSemaphore:%d\n",GetLastError());
	}
	printf("El hijo libera al semaforo\n");
	UnmapViewOfFile(shmHP);
	CloseHandle(hArchMapeoHP);
	CloseHandle(piH.hProcess);
	CloseHandle(piH.hThread);
	exit(0);//break;
}

