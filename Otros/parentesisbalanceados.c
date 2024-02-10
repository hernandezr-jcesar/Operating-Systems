#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nodo {
	char simbolo;
	struct nodo *sig;
};
struct nodo *raiz=NULL;
void insertar(char x){
	struct nodo *nuevo;
	nuevo = malloc(sizeof(struct nodo));
	nuevo->simbolo = x;
	if (raiz == NULL){
		raiz = nuevo;
		nuevo->sig = NULL;
	}else{
		nuevo->sig = raiz;
		raiz = nuevo;
	}
}
char extraer(){
	if(raiz != NULL){
		char informacion = raiz->simbolo;
		struct nodo *temp = raiz;
		raiz = raiz->sig;
		free(temp);
		return informacion;
	}else{return -1;}
}
void liberar(){
	struct nodo *tempo = raiz;
	struct nodo *temp;
	while(tempo != NULL){
		temp = tempo;
		tempo = tempo->sig;
		free(temp);
	}
}
int vacia(){
	if(raiz == NULL){
		return 1;
	}else{return 0;}
}
void cargarFormula(char *formula){
	printf("Ingrese la formula:");
	fgets(formula,100,stdin);
}
int verificarBalance(char *formula){	
	int f;
	for (f=0;f < strlen(formula);f++){
		if(formula[f] == '(' || formula[f] == '[' || formula[f] == '{'){
			insertar(formula[f]);
		}else{
			if(formula[f] == ')'){
				if(extraer()!= '('){return 0;}
			}else{
				if(formula[f] == ']'){
					if(extraer() != '['){return 0;}
				}else{
					if(formula[f] == '}'){
						if(extraer() != '{'){return 0;}
					}
				}
			}
		}	
	}
	if (vacia()){return 1;}
	else{return 0;}
}
int main(){
	char formula[100];
	cargarFormula(formula);
	if(verificarBalance(formula)){
		printf("El grupo de paréntesis SI esta balanceado\n");
	}else{
		printf("El grupo de paréntesis NO esta balanceado\n");
	}
	liberar();	
	return 0;
}
