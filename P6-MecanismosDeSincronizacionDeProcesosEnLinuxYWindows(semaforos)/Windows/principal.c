#include <stdio.h>
#include <windows.h>
#include "math.h"
#include "time.h"
#include "fun.h"
#define TAM_MEM 400
#define TAM 10
#define SWAP(a,b) {temp = (a);(a) = (b);(b) = temp;}

HANDLE crearArchivo(char* direccion);
BOOL write(HANDLE hFile,char* texto);
void stchcat(char *cadena, char chr);
void ImpMatriz(float** a);
float** generar();
float** toPointer(float a[TAM][TAM]);
double Determinante (float m1[TAM][TAM],int n);
void matrizCofactores (float m1[TAM][TAM], int n, double determinante);
void matrizCofactores2 (float m1[TAM][TAM], int n, double determinante);
  

int main(int argc,char *argv[]){
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	float A[10][10], B[10][10], b[10][10], c[10][10], d[10][10], mandada1[10][10], mandada2[10][10];
	char *PH = "PadreHijo";
	char *HP = "HijoPadre";
	char *NP = "NietoPadre";
	srand(time(NULL));
	HANDLE hArchMapeoPH, hArchMapeoHP, hArchMapeoNP,hSemaforo;
	int i,j,k,*aPH, *aHP, *aNP,*shmPH, *shmHP, *shmNP;
	int ct=0;
	
	if((hSemaforo=CreateSemaphore(NULL,1,1,"Semaforo"))==NULL){
		printf("Falla al invocar CreateSemaphore:%d\n",GetLastError());
		return -1;
	}
	puts("Se creo el semaforo"); 
		
 	if (!CreateProcess(NULL, argv[1], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)){
		printf("Fallo al invocar CreateProcess(%d)\n", GetLastError());
		return -1;
	}
	puts("Se creo el proceso hijo");
		
	if ((hArchMapeoPH = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, TAM_MEM, PH)) == NULL){
		printf("No se mapeo la memoria compartida: (%i)\n", GetLastError());
		exit(-1);
	}
	puts("Se creo el mapeo padre");
	
	if ((shmPH = (int *)MapViewOfFile(hArchMapeoPH, FILE_MAP_ALL_ACCESS, 0, 0, TAM_MEM)) == NULL){
		printf("No se creo la memoria compartida: (%i)\n", GetLastError());
		CloseHandle(hArchMapeoPH);
		exit(-1);
	}	
	aPH = shmPH;	
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			*aPH = rand() % 15;
			mandada1[i][j] = *aPH;
			*aPH++;
		}
	}
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			*aPH = rand() % 15;
			mandada2[i][j] = *aPH;
			*aPH++;
		}
	}
	*aPH = 101;
	while (*shmPH != -1){
		puts("Esperando Padre->Hijo");
//		sleep(1);
	}
	WaitForSingleObject(hSemaforo,INFINITE);
	
	printf("2 MATRICES. PADRE -> HIJO. PADRE.\nMatriz 1.\n");
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			printf("%d\t", mandada1[i][j]);
		}
		printf("\n");
	}
	printf("Matriz 2\n");
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			printf("%d\t", mandada2[i][j]);
		}
		printf("\n");
	}

	//WaitForSingleObject(hSemaforo,INFINITE);
	UnmapViewOfFile(shmPH);
	CloseHandle(hArchMapeoPH);

	if((hSemaforo=OpenSemaphore(SEMAPHORE_ALL_ACCESS,TRUE,"Semaforo"))==NULL){
		printf("Falla al invocar OpenSemaphore nieto:%d\n",GetLastError());
	}	
	puts("Recibiendo del hijo");
	
	if ((hArchMapeoHP = OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, HP)) == NULL){
		printf("No se ario archsadfadsfdfdfdivo de mapeo de la memoria: (%i)\n", GetLastError());
		exit(-1);
	}
	if ((shmHP = (int *)MapViewOfFile(hArchMapeoHP, FILE_MAP_ALL_ACCESS, 0, 0, TAM_MEM)) == NULL){
		printf("No se accedio a la memoria compartida: (%i)\n", GetLastError());
		CloseHandle(hArchMapeoHP);
		exit(-1);
	}
	aHP = shmHP;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			A[i][j] = *aHP;
			*aHP++;
		}
	}
	*shmHP = -1;
	
	printf("PRODUCTO. HIJO -> PADRE. PADRE.\n");
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			printf("%d\t", A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	UnmapViewOfFile(shmHP);
	CloseHandle(hArchMapeoHP);
	if(!ReleaseSemaphore(hSemaforo,1,NULL)){
		printf("Falla al invocar ReleaseSemaphore: %d\n",GetLastError());
	}

	//RECIBE MATRIZ DEL NIETO
	
	if((hSemaforo=OpenSemaphore(SEMAPHORE_ALL_ACCESS,TRUE,"Semaforo"))==NULL){
		printf("Falla al invocar OpenSemaphore nieto:%d\n",GetLastError());
	}

	if ((hArchMapeoNP = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, NP)) == NULL){
		printf("No se ario PADRE archivo de mapeo de la memoria: (%i)\n", GetLastError());
		exit(-1);
	}
	if ((shmNP = (int *)MapViewOfFile(hArchMapeoNP, FILE_MAP_ALL_ACCESS, 0, 0, TAM_MEM)) == NULL){
		printf("No se accedio a la memoria compartida: (%i)\n", GetLastError());
		CloseHandle(hArchMapeoNP);
		exit(-1);
	}
	aNP = shmNP;
	for (i = 0; i < 10; i++){
		for (j = 0; j < 10; j++){
			B[i][j] = *aNP++;
		}
	}
	*shmNP = -1;
	printf("SUMA. NIETO -> PADRE. PADRE.\n");
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			printf("%d\t", B[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	UnmapViewOfFile(shmNP);
	CloseHandle(hArchMapeoNP);

	
	if(Determinante(A,10)!=0){
	matrizCofactores(A,10,Determinante(A,10));
    }
	else{
		printf("La matriz producto no tiene inversa");
	}
	if(Determinante(B,10)!=0){
		matrizCofactores2(B,10,Determinante(B,10));
	}
	else{
		printf("La matriz suma no tiene inversa");
	}

	ReleaseSemaphore(hSemaforo,1,NULL);
		//printf("Falla al invocar ReleaseSemaphore: %d\n",GetLastError());
	printf("El padre libero el semaforo\n");
	Sleep(3000);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}
	
HANDLE crearArchivo(char* direccion){
HANDLE hFile;
hFile=CreateFile(direccion,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
return hFile;
}

BOOL write(HANDLE hFile,char* texto){
BOOL bFile;
DWORD numberofbytes=(DWORD)strlen(texto);
DWORD numberwritten=0;
bFile= WriteFile(hFile,texto,numberofbytes,&numberwritten,NULL);
return bFile;
} 

void stchcat(char *cadena, char chr){
   size_t longitud = strlen(cadena);   
   *(cadena + longitud) = chr;
   *(cadena + longitud + 1) = '\0';
}

void ImpMatriz(float** a){
     int i,j;
    for(i=0;i<TAM;i++){
    for(j=0;j<TAM;j++){
    printf("%.1f|\t",a[i][j]);
      }
    printf("\n");
    }
}

float** generar(){
   int i;
   float** a=(float**)malloc(sizeof(float*)*TAM);
   for(i=0;i<TAM;i++){
   a[i]=(float*)malloc(sizeof(float)*TAM);
   }
   return a;
}

float** toPointer(float a[TAM][TAM]){
        float** x=generar();
        int i,j;
       for(i=0;i<TAM;i++){
       for(j=0;j<TAM;j++){
       x[i][j]=a[i][j];
}
}
return x;
}  

double Determinante (float m1[TAM][TAM],int n){
  double s = 1, det = 0;
  int i, j, k, m, x;
  HANDLE hFile;
  float m2[TAM][TAM];
  if (n == 1)
  {
    return m1[0][0];
  }
  else
  {
    for (k = 0; k < n; k++)
    {
      m = 0;
      x = 0;
      for (i = 0; i < n; i++)
      {
        for (j = 0; j < n; j++)
        {
          m2[i][j] = 0;
          if (i != 0 && j != k)
          {
            m2[m][x] = m1[i][j];
            if (x < (n - 2))
            {
              x++;
            }
			else
            {
              x = 0;
              m++;
            }
          }
        }
      }
      det = det + s * (m1[0][k] * Determinante(m2, n-1));
      s *= -1;
    }
  }
  return det;
}  

void matrizCofactores (float m1[TAM][TAM], int n, double determinante){
  char resultado[700];
  char buffer[8]={};
  int q, m, i, j, k, l;
  HANDLE hFile;
  float m2[TAM][TAM], matrizFactores[TAM][TAM], aux[TAM][TAM], inversa[TAM][TAM];
  for (l = 0; l < n; l++)
  {
    for (k = 0; k < n; k++)
    {
      m = 0; q = 0;
      for (i = 0; i < n; i++)
      {
        for (j = 0; j < n; j++)
        {
          if ( i != l && j != k)
          {
            m2[m][q] = m1[i][j];
            if (q < (n-2))
            {
              q++;
            }
			else 
            {
				q = 0; m++;
            }
          }
        }
      }
      matrizFactores[l][k] = (pow(-1, l + k)* Determinante (m2, n-1));
    }
  }
  for (i = 0; i < TAM; i++)
  {
    for (j = 0; j < TAM; j++)
    {
      aux[i][j] = matrizFactores[j][i];
    }
  }
  for (i = 0; i < TAM; i++)
  {
    for (j = 0; j < TAM; j++)
    {
      inversa[i][j] = aux[i][j] / determinante;
}
  }
   strcpy(resultado,"\r\nMATRIZ INVERSA A\r\n ");
  for (i = 0; i < TAM; i++)
  {
    for (j = 0; j < TAM; j++)
    {
	  sprintf(buffer,"%.3lf\t",inversa[i][j]);
	  strcat(resultado,buffer);
	 memset(buffer,0,8);
    }
strcat(resultado,"\r\n");
  }
  printf("%s\n",resultado);
 DWORD noReaded=0;
 hFile=crearArchivo("C:\\Users\\Principal\\Documents\\4to Semestre\\Sistemas Operativos\\Practica7\\inversa1.txt");
 BOOL bFile=write(hFile,resultado);
 
}
void matrizCofactores2 (float m1[TAM][TAM], int n, double determinante){
  char resultado[700];
  char buffer[8]={};
  HANDLE hFile;
  int q, m, i, j, k, l;
  float m2[TAM][TAM], matrizFactores[TAM][TAM], aux[TAM][TAM], inversa[TAM][TAM];
  for (l = 0; l < n; l++)
  {
    for (k = 0; k < n; k++)
    {
      m = 0; q = 0;
      for (i = 0; i < n; i++)
      {
        for (j = 0; j < n; j++)
        {
          if ( i != l && j != k)
          {
            m2[m][q] = m1[i][j];
            if (q < (n-2))
            {
              q++;
            }
			else 
            {
				q = 0; m++;
            }
          }
        }
      }
      matrizFactores[l][k] = (pow(-1, l + k)* Determinante (m2, n-1));
    }
  }
  for (i = 0; i < TAM; i++)
  {
    for (j = 0; j < TAM; j++)
    {
      aux[i][j] = matrizFactores[j][i];
    }
  }
  for (i = 0; i < TAM; i++)
  {
    for (j = 0; j < TAM; j++)
    {
      inversa[i][j] = aux[i][j] / determinante;
    }
  }
  
  /*****************IMPRESION DE LA INVERSA************/
  strcpy(resultado,"\r\nMATRIZ INVERSA B \r\n");
  for (i = 0; i < TAM; i++)
  {
    for (j = 0; j < TAM; j++)
    {
	  sprintf(buffer,"%.3lf\t",inversa[i][j]);
	  strcat(resultado,buffer);
	  memset(buffer,0,8);
    }
	strcat(resultado,"\r\n");
  }
  printf("%s\n",resultado);
   hFile=crearArchivo("C:\\Users\\Principal\\Documents\\4to Semestre\\Sistemas Operativos\\Practica7\\inversa2.txt");
 BOOL bFile=write(hFile,resultado);
}

