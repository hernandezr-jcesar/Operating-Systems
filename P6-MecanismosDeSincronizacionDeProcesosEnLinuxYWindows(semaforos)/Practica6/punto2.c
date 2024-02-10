/*
	semget() -> Crear un grupo de Semaforos dentro de un grupo
	
	semop()	-> Es la llamada al sistema que permite realizar las operaciones
	que se vana a efectuar en un semaforo
	las dos operaciones basicas de los semaforos:
		-esperar -> verificar en que condicion se encuentra la variable del semaforo
			-> Dependiendo del valor de la variable del semaforo se tienen dos posibilidades:
				-DESOCUPADO: El proceso puede ingresar a la sección critica(puede empezar a trabajar
				el codigo protegido por el semaforo)
				-OCUPADO: Va a tener que hacer la espera del semaforo
		-señal(signal)
			->La variable del semaforo se regresa a un valor de desocupado, (se hace cuando el proceso sale de la
			seccion critica, del codigo protegido), ademas le avisa a otros procesos que el semaforo ya esta libre, 
			para que el que lo necesite lo use.
	(Un semaforo no es mas que una variable que puede tomar valores dependiendo de los tipos
	de semaforo: de conteo(varios valores), o uno binario(solo dos valores 0 1))

	
*/
#include <stdio.h>
#include <sys/types.h> // Son para manejar estructuras de datos
#include <sys/ipc.h> //   auxiliares que apoyan a sem.h
#include <sys/sem.h> //las llamadas al sistema para los semaforos
int main(void)
{
	int i,j;
	int pid;
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
		while (i < 3) { //Las 3 veces que el proceso va a entrar a la seccion critica
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
			printf("semop: hijo llamando a semop(%d, &sops, %d) con:", semid, nsops);
			for (j = 0; j < nsops; j++) {
				printf("\n\tsops[%d].sem_num = %d, ", j, sops[j].sem_num);
				printf("sem_op = %d, ", sops[j].sem_op);
				printf("sem_flg = %#o\n", sops[j].sem_flg);
			}
			if ((j = semop(semid, sops, nsops)) == -1) {
				perror("semop: error en operacion del semaforo\n");
			}
			else {
				printf("\tsemop: regreso de semop() %d\n", j);
				printf("\n\nProceso hijo toma el control del semaforo: %d/3 veces\n", i+1);
				sleep(5); // Aqui es para simular lo que el proceso hijo estaria ejecutando como parte de su seccion critica, como lo es acceso a memoria compartida, acceso a un archivo compartido
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
					printf("Proceso hijo regresa el control del semaforo: %d/3 veces\n", i+1);
				sleep(5);//Simula otras operaciones fuera de la seccion critica
			}
			++i;
		}
	}
	//PROCESO PADRE
	else {
		i = 0;
		while (i < 3) {
			nsops = 2;
			sops[0].sem_num = 0;
			sops[0].sem_op = 0;
			sops[0].sem_flg = SEM_UNDO;

			sops[1].sem_num = 0;
			sops[1].sem_op = 1;
			sops[1].sem_flg = SEM_UNDO | IPC_NOWAIT;
			printf("\nsemop: Padre llamando semop(%d, &sops, %d) con:", semid, nsops);
			for (j = 0; j < nsops; j++) {
				printf("\n\tsops[%d].sem_num = %d, ", j, sops[j].sem_num);
				printf("sem_op = %d, ", sops[j].sem_op);
				printf("sem_flg = %#o\n", sops[j].sem_flg);
			}
			if ((j = semop(semid, sops, nsops)) == -1) {
				perror("semop: error en operacion del semaforo\n");
			}else {
				printf("semop: regreso de semop() %d\n", j);
				printf("Proceso padre toma el control del semaforo: %d/3 veces\n", i+1);
				sleep(5);
				nsops = 1;
				sops[0].sem_num = 0;
				sops[0].sem_op = -1;
				sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT;
				if ((j = semop(semid, sops, nsops)) == -1) {
					perror("semop: error en semop()\n");
				}
				else
					printf("Proceso padre regresa el control del semaforo: %d/3 veces\n", i+1);
				sleep(5);
			}
			++i;
		}
	}
}
