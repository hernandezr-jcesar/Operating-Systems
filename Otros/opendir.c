#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(int argc, char const *argv[])
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
		for(int conta=0;conta < strlen(cadena);conta++){			
			if(cadena[conta] == '.'){
				if(cadena[conta+1] == 't'){
					if(cadena[conta+2] == 'x'){
						if(cadena[conta+3] == 't'){

							printf("-> %s\n", sd->d_name);

						}
					}

				} 
			}

		}
	}
	/*
	char cadena[100];
	printf("Favor de ingresar el nombre del archivo a especificar:\n");
	scanf("%s",cadena);

	printf("%s\n",cadena);
	*/

	return 0;
}