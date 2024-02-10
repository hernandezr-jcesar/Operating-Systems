// Programa en C++ para 
// evaluar una expression dada donde
// los simbolos estan separados con
// un espacio
#include <bits/stdc++.h>
using namespace std;

int precedencia(char op){
	if(op == '+'||op == '-')
	return 1;
	if(op == '*'||op == '/')
	return 2;
	return 0;
}

int aplicarOper(int a, int b, char op){
	switch(op){
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/': return a / b;
	}
	return 0;
}

int evaluar(string simbolos){
	int i;
	stack <int> valores;
	stack <char> ops;
	
	for(i = 0; i < simbolos.length(); i++){
		
		if(simbolos[i] == ' ')
			continue;
			
		else if(simbolos[i] == '('){
			ops.push(simbolos[i]);
		}
		else if(isdigit(simbolos[i])){
			int val = 0;
		
			while(i < simbolos.length() && isdigit(simbolos[i]))
			{
				val = (val*10) + (simbolos[i]-'0');
				i++;
			}
			valores.push(val);
			i--;
		}
		
		else if(simbolos[i] == ')')
		{
			while(!ops.empty() && ops.top() != '(')
			{
				int val2 = valores.top();
				valores.pop();
				
				int val1 = valores.top();
				valores.pop();
				
				char op = ops.top();
				ops.pop();
				
				valores.push(aplicarOper(val1, val2, op));
			}
			if(!ops.empty())
			ops.pop();
		}
		else
		{
			while(!ops.empty() && precedencia(ops.top()) >= precedencia(simbolos[i])){
				int val2 = valores.top();
				valores.pop();
				
				int val1 = valores.top();
				valores.pop();
				
				char op = ops.top();
				ops.pop();
				
				valores.push(aplicarOper(val1, val2, op));
			}
			ops.push(simbolos[i]);
		}
	}
	while(!ops.empty()){
		int val2 = valores.top();
		valores.pop();
				
		int val1 = valores.top();
		valores.pop();
				
		char op = ops.top();
		ops.pop();
				
		valores.push(aplicarOper(val1, val2, op));
	}
	return valores.top();
}

int main() {
	string expresion;
	cout << ("Ingresa la Expresion Aritmetica: \n");
	cin >> expresion;
	cout <<  "\n";
	cout << "Resultado: "<<evaluar(expresion) << "\n";
	return 0;
}
