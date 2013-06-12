#include <stdio.h>
#include "Hw4Interface.h"


FILE* Open (char* filename, char* mode)
{
	FILE* file;
	if((file = fopen(filename, mode)) == NULL)
		printf("\nUnable to open \"%s\".", filename);

	return file;
}


void ReadSpecification(FILE*  file, int* rows, int* columns, int* output)
{
	if(output !=NULL)
		fscanf(file, "%d%d%d",&(*rows), &(*columns), &(*output));
	else
		fscanf(file, "%d%d",&(*rows), &(*columns));
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


void Free(double** Matrix, int rows)
{
	int i;
	for(i = 0; i < rows; i++)
		free(Matrix[i]);
			
	free(Matrix);
}


double*** MakeWeightsAndBiasMatrices(FILE* init_weights, double*** Matrix, int w_input, int w_output, double wrange)
{
	int i,j;
	
	if((Matrix[2] = (double **) malloc(w_output * sizeof (double *))) == NULL ||
		(Matrix[3] = (double **) malloc(1 * sizeof (double *))) == NULL)  
	{
		printf("\nFatal malloc error!");
		exit(1);
	}

	if((Matrix[3][0] = (double *) malloc(w_output * sizeof (double))) == NULL)
	{
		printf("\nFatal malloc error!");
		exit(1);
	}

	if(init_weights != NULL)
		for (i = 0; i< w_output; i++)
			fscanf(init_weights, "%lg", &Matrix[3][0][i]);
	else
		Matrix[3] = RandomInit(Matrix[3], 1, w_output, wrange);

	for (i = 0; i < w_output; i++)
	{
	
		if((Matrix[2][i] = (double *) malloc(w_input * sizeof (double))) == NULL)
		{
			printf("\nFatal malloc error!");
			exit(1);
		}
		
		if(init_weights == NULL)
			Matrix[2] = RandomInit(Matrix[2], w_output, w_input, wrange);
		else
			for(j = 0; j < w_input; j++)
				fscanf(init_weights,"%lg", &Matrix[2][i][j]);
	}

	return Matrix;
}

double** RandomInit(double** Matrix, int rows, int columns, double wrange)
{
	int i,j;
	
	for(i = 0; i < rows; i++)
		for(j = 0; j < columns; j++)
			Matrix[i][j] = (drand48() - 0.5) * 2.0 * wrange;
		
	return Matrix;
}


double*** MakeOutputMatrix(double*** Matrices, int rows, int output)
{
	int i;

	Matrices[4] = (double **) malloc (rows * sizeof(double*));
	
	for(i = 0; i < rows; i++)
		Matrices[4][i] = (double*) malloc (output * sizeof(double));
	
	return Matrices;
}


double CalcNet(double* PMatrix, double* WMatrix, double bias, int input_size)
{
	int i;
	double output = 0;

	for(i = 0; i < input_size; i++)
		output += PMatrix[i] * WMatrix[i];
	
	return output + bias;
}


void ChangeWeights(double* WMatrix, double* PMatrix, double Output, double Teacher, int input_size)
{
	int i;

	for(i = 0; i < input_size; i++)
		WMatrix[i] = WMatrix[i] + (Teacher - Output) * PMatrix[i];
}


void ChangeBias(double* BMatrix, double Output, double Teacher)
{
	*BMatrix += Teacher - Output;
}


void Save(char* swfile, double** WMatrix, double* BMatrix, int rows, int output)
{
	FILE*	file;
	int i,j;

	file = fopen(swfile, "w");
	fprintf(file, "%d\n%d\n", rows, output);
	
	for( j = 0; j < rows; j++)
		fprintf(file, "%g  ", BMatrix[j]);
	
	for( i = 0; i < output; i++)
	{
		fprintf(file, "\n");
		for( j = 0; j < rows; j++)
			fprintf(file, "%g  ", WMatrix[i][j]);
	}
}
