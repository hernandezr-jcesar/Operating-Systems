#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

double** startMatrix(int* nSize){
	int i;

	double** newMatrix = calloc(*nSize, sizeof(double*));
	
	for(i = 0; i < *nSize; ++i)
		newMatrix[i] = calloc(*nSize, sizeof(double));
	
	return newMatrix;
}
double** addMatrix(double** matrixA, double** matrixB, int nSize){
	int i, j;
	double** resultMatrix = startMatrix(&nSize);

	for(i = 0; i < nSize; ++i)
		for(j = 0; j < nSize; ++j)
			resultMatrix[i][j] = matrixA[i][j] + matrixB[i][j];

	return resultMatrix;
}

double** randomizeMatrix(int* nSize){
	int i,j;
	double** randomMatrix = startMatrix(nSize);

	for(i = 0; i < *nSize; ++i)
		for(j = 0; j < *nSize; ++j){
			randomMatrix[i][j] = -10+(20. * rand())/RAND_MAX;
			while (randomMatrix[i][j] == 0)
			{
				randomMatrix[i][j] = -10+(20. * rand())/RAND_MAX;
			}
		}
	return randomMatrix;
}

void printMatrix(double** matrix, int* nSize){
	int i, j;

	printf("\n");
	for(i = 0; i < *nSize; ++i){
		for(j = 0; j < *nSize; ++j)
			printf("%0.3lf\t", matrix[i][j]);
		printf("\n");
	}
}

void copyMatrix(int* nSize, double** origin, double** destiny){
	int i, j;

	for (i = 0; i < *nSize; ++i)
		for (j = 0; j < *nSize; ++j)
			destiny[i][j] = origin[i][j];
}

void writeMatrixFile(double** matrix, char* name, int nSize){
	FILE * auxFile = fopen(name, "w");

	for(int i = 0; i < nSize; ++i){
		for(int j = 0; j < nSize; ++j)
			fprintf(auxFile, "%0.3lf ", matrix[i][j]);
		fprintf(auxFile, "\n");
	}
	fclose(auxFile);
}

double** readMatrixFile(char* name, int nSize){
	double** matrix = startMatrix(&nSize);
	FILE * auxFile = fopen(name, "r");

	for(int i = 0; i < nSize; ++i)
		for(int j = 0; j < nSize; ++j)
			fscanf(auxFile, "%lf", &matrix[i][j]);
	fclose(auxFile);

	return matrix;
}

double** inverseMatrix(int nSize, double** matrix){
	double **invMatrix;
	int i, j, k;

	invMatrix = startMatrix(&nSize);
	copyMatrix(&nSize, matrix, invMatrix);

	if (nSize <= 1) return invMatrix;  // must be of dimension >= 2
	for (i=1; i < nSize; i++) invMatrix[0][i] /= invMatrix[0][0]; // normalize row 0
	for (i=1; i < nSize; i++)  { 
		for (j=i; j < nSize; j++)  { // do a column of L
			double sum = 0.0;
			for (k = 0; k < i; k++)  
				sum += invMatrix[j][k] * invMatrix[k][i];
			invMatrix[j][i] -= sum;
		}
		if (i == nSize-1) continue;
		for (j=i+1; j < nSize; j++)  {  // do a row of U
			double sum = 0.0;
			for (k = 0; k < i; k++)
				sum += invMatrix[i][k]*invMatrix[k][j];
			invMatrix[i][j] = (invMatrix[i][j]-sum) / invMatrix[i][i];
		}
	}
	for ( i = 0; i < nSize; i++ )  // invert L
		for ( j = i; j < nSize; j++ )  {
			double x = 1.0;
			if ( i != j ) {
				x = 0.0;
				for ( k = i; k < j; k++ ) 
					x -= invMatrix[j][k]*invMatrix[k][i];
			}
			invMatrix[j][i] = x / invMatrix[j][j];
		}
	for ( i = 0; i < nSize; i++ )   // invert U
		for ( j = i; j < nSize; j++ )  {
			if ( i == j ) continue;
			double sum = 0.0;
			for ( k = i; k < j; k++ )
				sum += invMatrix[k][j]*( (i==k) ? 1.0 : invMatrix[i][k] );
			invMatrix[i][j] = -sum;
		}
	for ( i = 0; i < nSize; i++ )   // final inversion
		for ( j = 0; j < nSize; j++ )  {
			double sum = 0.0;
			for ( k = ((i>j)?i:j); k < nSize; k++ )  
				sum += ((j==k)?1.0:invMatrix[j][k])*invMatrix[k][i];
			invMatrix[j][i] = sum;
		}

	return invMatrix;
}

double** prodMatrix(double** matrixA, double** matrixB, int nSize){
	int i, j, k;
	double** resultMatrix = startMatrix(&nSize);

	for(i = 0; i < nSize; ++i)
		for(j = 0; j < nSize; ++j)
			for(k = 0; k < nSize; ++k)
				resultMatrix[i][j] += matrixA[i][k] * matrixB[k][j];
	
	return resultMatrix;
}
