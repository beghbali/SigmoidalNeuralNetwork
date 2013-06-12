#include <stdio.h>
#include <stdlib.h>

void ReadSpecification(FILE*  file, int* rows, int* columns, int* output);
double*** MakePatternAndTeacherMatrices(FILE* file, double*** Matrix, int rows, int columns, int outputs, int NMatrices);
double*** MakeWeightsAndBiasMatrices(FILE* init_weights, double*** Matrix, int w_input, int w_output);
void Free(double** Matrix, int rows);
int main(void)
{
	FILE* file;
	FILE* init_weights;
	int rows, columns, output,i,j,w_input,w_output;
	int NMatrices = 4;
	double** PMatrix;
	double** TMatrix;
	double** WMatrix;
	double** BMatrix;
	double*** Matrices;

	
	
	file = fopen("or.pat", "r");
	ReadSpecification(file, &rows, &columns, &output);
	
	Matrices = MakePatternAndTeacherMatrices(file, Matrices, rows, columns, output, NMatrices);
	fclose(file);
	PMatrix = Matrices[0];
	TMatrix = Matrices[1];
	
	for(i = 0; i < rows; i++)
	{
		printf("\n");
		for(j = 0; j < columns; j++)
			printf("%g  ", PMatrix[i][j]);
		for(j = 0; j < output; j++)
			printf("%g  ", TMatrix[i][j]);
	}
	
	init_weights = fopen("wts.txt", "r");
	ReadSpecification(init_weights, &w_input, &w_output, NULL);

	Matrices = MakeWeightsAndBiasMatrices(init_weights, Matrices, w_input, w_output);
	WMatrix = Matrices[2];
	BMatrix = Matrices[3];
	printf("\n");
	for(j=0; j<w_output; j++)
		printf("%g",Matrices[3][0][j]);

	for(i=0; i<w_output; i++)
	{
		printf("\n");
		for(j=0; j<w_input; j++)
			printf("%g  ",Matrices[2][i][j]);
	}
	fclose(init_weights);
	Free(Matrices[0], rows);
	Free(Matrices[1], rows);
	Free(Matrices[2], w_output);
	Free(Matrices[3], 1);
	free(Matrices);
	return 0;
	
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
		printf("\n");
		if((Matrix[0][i] = (double *) malloc(columns * sizeof (double))) == NULL ||
			(Matrix[1][i] = (double *) malloc(outputs * sizeof (double))) == NULL)
		{
			printf("\nFatal malloc error!");
			exit(1);
		}
		else
		{
			for(j = 0; j < columns; j++)
			{
				fscanf(file,"%lg", &Matrix[0][i][j]);
				printf("%g ",Matrix[0][i][j]);
			}
			for(j = 0; j < outputs; j++)
			{
				fscanf(file, "%lg", &Matrix[1][i][j]);
				printf("%g ",Matrix[1][i][j]);
			}
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


double*** MakeWeightsAndBiasMatrices(FILE* init_weights, double*** Matrix, int w_input, int w_output)
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
	
	for (i = 0; i< w_output; i++)
		fscanf(init_weights, "%lg", &Matrix[3][0][i]);

	for (i = 0; i < w_output; i++)
	{
	
		if((Matrix[2][i] = (double *) malloc(w_input * sizeof (double))) == NULL)
		{
			printf("\nFatal malloc error!");
			exit(1);
		}
		else
			for(j = 0; j < w_input; j++)
				fscanf(init_weights,"%lg", &Matrix[2][i][j]);
	}
	return Matrix;
}

void ReadSpecification(FILE*  file, int* rows, int* columns, int* output)
{
	if(output !=NULL)
		fscanf(file, "%d%d%d",&(*rows), &(*columns), &(*output));
	else
		fscanf(file, "%d%d",&(*rows), &(*columns));
}