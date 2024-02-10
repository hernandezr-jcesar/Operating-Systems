//Si es un hilo terminal imprimir "Practica4"
//Si es un hilo padre imprimir los id de los hilos creados si es un hilo padre

//pid == 0  <- Proceso Hijo
//pid < 0  <- Proceso Padre
//pid == -1 <-Error
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <unistd.h>
#include <sys/types.h>

void hilo1();
int main(){
	pid_t pid;
	
	pid = fork();
	
	if(pid == 0){
		printf("Soy el proceso hijo(%d), hijo de(%d).\n",getpid(),getppid());
			pthread_t th1;
	
			//Creacion hilo1
			pthread_create(&th1,NULL,(void*)hilo1,NULL);
			pthread_join(th1,NULL);//Esperar a hilo1
		
	}else{
		printf("Soy el proceso padre(%d), hijo de(%d).\n",getpid(),getppid());
		
	}
	

}
void hilo1(){
	printf("Soy el Hilo 1 %lx\n",pthread_self());
	pthread_exit(NULL); //Finaliza la ejecución del hilo
}
