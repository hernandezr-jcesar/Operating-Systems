/*
 10. Una vez creados los archivos con sus 
 contenidos por el programa del punto 8 y 
 utilizando unicamente las llamadas al sistema 
 revisadas para Linux que sean necesarias, 
 desarrolar:
 un programa en C que liste los archivos 
 creados, mostrando su tamanio, fecha y hora de acceso.
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

//para poder usar el opendir, readdir
#include <stdlib.h>
#include <dirent.h>

//simulacion del comando ls
// comandos usados para este programa:
// stat, opendir, readdir 
int main()
{
	DIR *dir;
	struct dirent *sd;

	dir = opendir(".");

	if(dir == NULL)
	{
		printf("Error! No se puede abrir el directorio\n");
		exit(1);
	}
	char cadena[100];
	char temp[100];
	while ( (sd=readdir(dir)) != NULL )
	{
		//printf("-> %s\n", sd->d_name);
		//fputs(cadena,sd->d_name);
		sprintf(cadena,sd->d_name);
		int conta = 0;
		while(conta < strlen(cadena)){
			if(cadena[conta] == '.'){
				if(cadena[conta+1] == 't'){
					if(cadena[conta+2] == 'x'){
						if(cadena[conta+3] == 't'){

							printf("-> %s\n", sd->d_name);
							struct stat buf;
							char mtime[100];

							stat(sd->d_name, &buf);
							printf("Tamanio: %ld bytes\n",buf.st_size);
							strcpy(mtime, ctime(&buf.st_mtime));
							printf("Ultimo Acceso:  %s\n", mtime);
						}
					}

				} 
			}
			conta++;
		}
		
	}
	return 0;
}

