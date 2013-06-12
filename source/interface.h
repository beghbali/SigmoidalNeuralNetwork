#include <stdio.h>
#include <stdlib.h>

FILE* Open (char* filename, char* mode);

void ReadSpecification(FILE* file, int* rows, int* columns, int* output);

double*** MakePatternAndTeacherMatrices(FILE* file, double*** Matrix, int rows, int columns, int outputs, int NMatrices);

double*** MakeWeightsAndBiasMatrices(FILE* init_weights, double*** Matrix, int w_input, int w_output, double wrange);

void Free(double** Matrix, int rows);

double** RandomInit(double** Matrix, int rows, int columns, double wrange);

double*** MakeOutputMatrix(double*** Matrices, int rows, int output);

double CalcNet(double* PMatrix, double* WMatrix, double bias, int input_size);

void ChangeWeights(double* WMatrix, double* PMatrix, double Output, double Teacher, int input_size);

void ChangeBias(double* BMatrix, double Output, double Teacher);

void Save(char* swfile, double** WMatrix, double* BMatrix, int rows, int output);