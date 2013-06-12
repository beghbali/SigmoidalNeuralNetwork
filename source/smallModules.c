#include <stdio.h>
#include "interface.h"


FILE* open (char* filename, char* mode)
{
	FILE* file;

	if((file = fopen(filename, mode)) == NULL)
		printf("\nUnable to open \"%s\".", filename);

	return file;
}


void ReadPatternSpecification(FILE*  file, int rows, int columns, int output)
{
	fscanf(file, "%d%d%d",&rows, &columns, &output);
}


double***  MakePatternAndTeacherMatrices(FILE* file, double*** Matrix, int rows, int columns, int outputs, int NMatrices)
{
	int i,j;
	
	if((Matrix = (double ***) malloc(NMatrices * sizeof (double **))) == NULL ||
		(Matrix[0] = (double **) malloc(rows * sizeof (double *))) == NULL ||
		(Matrix[1] = (double **) malloc(rows * sizeof (double *))) == NULL)
	{
		printf("\nFatal malloc error!");
		exit(1);
	}


	for (i = 0; i < rows; i++)
	{

		if((Matrix[0][i] = (double *) malloc(columns * sizeof (double))) == NULL ||
			(Matrix[1][i] = (double *) malloc(outputs * sizeof (double))) == NULL)
		{
			printf("\nFatal malloc error!");
			exit(1);
		}
		else
		{
			for(j = 0; j < columns; j++)
				fscanf(file,"%lg", &Matrix[0][i][j]);
	
			for(j = 0; j < outputs; j++)
				fscanf(file, "%lg", &Matrix[1][i][j]);
		}	
	}
	return Matrix;
}


void Free(double*** Matrix, int NMatrices)
{
	int i,j;
	for(i = 0; i < NMatrices; i++)
	{
		for(j = 0; j < sizeof(**Matrix); j++)
			free(Matrix[i][j]);
		free(Matrix[i]);
	}
	
	free(Matrix);
}