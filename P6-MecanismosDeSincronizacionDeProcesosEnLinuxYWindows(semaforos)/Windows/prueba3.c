#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#define TAM_MEM 400

int main(int argc, char *argv[]){
	int A[10][10], B[10][10], b[10][10], suma[10][10], d[10][10], mandada1[10][10], mandada2[10][10];
	char *NP = "NietoPadre";
	char *HN = "HijoNieto";
	HANDLE hArchMapeoNP, hArchMapeoHN,hSemaforo;
	int i, j, k, shmid,ct=0;
	int *aHN, *aNP;
	srand(time(NULL));
	int *shmNP, *shmHN;
	
	if((hSemaforo=OpenSemaphore(SEMAPHORE_ALL_ACCESS,TRUE,"Semaforo"))==NULL){
		printf("Falla al invocar OpenSemaphore nieto:%d\n",GetLastError());
	}

	//RECIBE MATRIZ DEL HIJO
	if ((hArchMapeoHN = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, HN)) == NULL){
		printf("No se abrio en el nieto archivo de mapeo de la memoria: (%i)\n", GetLastError());
		exit(-1);
	}
	if ((shmHN = (int *)MapViewOfFile(hArchMapeoHN, FILE_MAP_ALL_ACCESS, 0, 0, TAM_MEM)) == NULL){
		printf("No se accedio en el nieto a la memoria compartida: (%i)\n", GetLastError());
		CloseHandle(hArchMapeoHN);
		exit(-1);
	}
	aHN = shmHN;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			A[i][j] = *aHN;
			aHN++;
		}
	}	
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			B[i][j] = *aHN;
			aHN++;
		}
	}
	*shmHN = -1;
	printf("2 MATRICES. HIJO -> NIETO. NIETO.\nMatriz 1\n");
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
	UnmapViewOfFile(shmHN);
	CloseHandle(hArchMapeoHN);
	//HACE LA SUMA
	for (i = 0; i<10; i++){
		for (j = 0; j<10; j++){
			suma[i][j] = A[i][j] + B[i][j];
		}
	}
	//MANDA MATRIZ AL PADRE

	if ((hArchMapeoNP = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, TAM_MEM, NP)) == NULL){
		printf("No se mapeo la memoria compartida en el nieto: (%i)\n", GetLastError());
		exit(-1);
	}
	if ((shmNP = (int *)MapViewOfFile(hArchMapeoNP, FILE_MAP_ALL_ACCESS, 0, 0, TAM_MEM)) == NULL)
	{
		printf("No se creo la memoria compartida en el nieto: (%i)\n", GetLastError());
		CloseHandle(hArchMapeoNP);
		exit(-1);
	}
	aNP = shmNP;
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			*aNP = suma[i][j];
			*aNP++;
		}
	}
	printf("SUMA. NIETO -> PADRE. NIETO.\n");
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			printf("%d\t", suma[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*aNP = 101;
	while (*shmNP != -1){
		puts("Esperando Nieto->Padre");
//		sleep(1);
	}
	WaitForSingleObject(hSemaforo,INFINITE);
	UnmapViewOfFile(shmNP);
	CloseHandle(hArchMapeoNP);
	if(!ReleaseSemaphore(hSemaforo,1,NULL)){
		printf("Falla al invocar ReleaseSemaphore:%d\n",GetLastError());
	}
	printf("El nieto libera el semaforo\n");
//	Sleep(3000);

	exit(0);
}
