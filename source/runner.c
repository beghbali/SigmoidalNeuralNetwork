#include <stdio.h>
//#include <unistd.h>
//#include <sys/time.h>         
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "smallModules.h"


int main(argc,argv)
int argc; char **argv;
{

long	nsweeps = -1;	/* number of sweeps to run for -- by default, */
                        /*    train until convergence                 */
char	patfile[128] ;	/* pattern filename */
char	iwfile[128]  ;	/* initial weights filename */
char	swfile[128]  ;	/* save weights filename */
char	tlfile[128]  ;	/* test log filename */
char	sdfile[128]  ;	/* seed log filename */


int     test = 0;       /* if set, the vector of output activations     */
                        /* for each pattern using the current weights   */ 
                        /* is saved to a log file -- no learning occurs */ 

int     c;

int rows,					/*# rows of the pattern file*/
	columns,				/*# columns of the pattern file*/
	output,					/*# outputs for the pattern file*/
	i,
	j,
	w_input,				/*# rows in the weights file*/
	w_output,				/*# columns in the weights file*/
	epochCount = 1;			/*counts the # of epoch*/

int NMatrices = 5;		/*# of matrices in Matrices*/
int wrong = 0;			/*if 1 then an error occured in a pattern row*/
int N_incorrect = 0;	/*# of incorrect patterns*/

double  wrange = 0.1;   /* random initial weights are set to values  */
                        /* between -wrange and +wrange               */
double**	PMatrix;	/*Pattern Matrix*/
double**	TMatrix;	/*Teacher Matrix*/
double**	WMatrix;	/*Weights Matrix*/
double**	BMatrix;	/*Biases Matrix*/
double**    OMatrix;	/*Output Matrix*/
double***	Matrices;	/*An array that holds all the Matrices above*/

short weights_initialized = 0;		/*if non-zero then weight matrix has been initialized from a file*/
short Converge = 0;					/*if non-zero then the perceptron has converged*/
short save_weights = 0;				/*if non-zero then the weights will be saved in the filename*/
									/*provided by -s option*/
	
/* seed for random number generator */
long	seed = ((time(NULL)&0x0000ffff) | (getpid()<<16)); 


FILE	*cmdfp;
FILE	*seedfp;
FILE    *patternFile;		/*for the pattern file*/
FILE    *init_weights;		/*for the weight file*/
FLLE	*test;				/*for the test file*/

while ((c = getopt(argc, argv, "p:l:s:n:t:r:S:")) != EOF) 
{

	switch (c) 
	{
		case 'p':
			strcpy(patfile,optarg);
			patternFile= Open(patfile, "r");
			
			/*reading the pattern file specifications to configure the Matrices*/
			ReadSpecification(patternFile, &rows, &columns, &output);
			
			/*Making the Pattern and Teacher matrices form a file*/
			Matrices = MakePatternAndTeacherMatrices(patternFile, Matrices, rows, columns, output, NMatrices);

			fclose(patternFile);
			
			PMatrix = Matrices[0];			/*pattern matrix*/
			TMatrix = Matrices[1];			/*teacher matrix*/
				
			break;
		case 'l':
			strcpy(iwfile,optarg);
			init_weights = fopen(iwfile, "r");
	
			/*reading the weight file specifications to configure the matrices*/
			ReadSpecification(init_weights, &w_input, &w_output, NULL);
			
			/*Making the Weight and Bias matrices from a file*/
			Matrices = MakeWeightsAndBiasMatrices(init_weights, Matrices, w_input, w_output,0);
			fclose(init_weights);

			WMatrix = Matrices[2];			/*weight matrix*/
			BMatrix = Matrices[3];			/*bias matrix*/

			weights_initialized = 1;	/*weights are initialized from a file*/
			
			break;
		case 's':
			strcpy(swfile,optarg);
			save_weights = 1;
			break;
		case 'n':
			nsweeps = (long) atol(optarg);
			break;
		case 't':
			strcpy(tlfile,optarg);
                        test = 1;
			break;
		case 'r':
                        wrange = (double) atof(optarg); 
                        break;
		case 'S':
		  	strcpy(sdfile,optarg);
			if(strlen(sdfile)<1)
                           exit(2);
                        if(seedfp = fopen(sdfile,"r"))
                        /* file exists - read in seed */
                           {
                           seedfp = fopen(sdfile,"r");
                           fscanf(seedfp,"%ld", &seed);
                           fclose(seedfp);
                           } 
                        else
                        /* save the random seed */
						{
                           seedfp = fopen(sdfile,"w");
                           fprintf(seedfp,"%ld", seed);
                           fclose(seedfp);
						}
				break;
		default:
			exit(2);
		break;
	}
}

/*if a weight file is not specified to initialize the weight matrix*/
/*the following is executed										   */

if(weights_initialized == 0)
{
	Matrices = MakeWeightsAndBiasMatrices(NULL, Matrices, columns, output, wrange);

	WMatrix = Matrices[2];           /*weight matrix*/
	BMatrix = Matrices[3];           /*Bias matrix*/
}

Matrices = MakeOutputMatrix(Matrices, rows, output);

OMatrix = Matrices[4];			/*output matrix*/

if(test = 1)					/*for the -t option*/
	test = fopen(tlfile, "w");
	
while (!(Converge || epochCount == nsweeps+1))
{	
	N_incorrect = 0;
 
	for(i = 0; i < rows; i++)
	{
		for(j = 0; j < output; j++)
		{
			
			/*calculate the net input to output[i] and determine the output*/
			if((CalcNet(PMatrix[i], WMatrix[j], BMatrix[0][j], columns)) >= 0)
				OMatrix[i][j] = 1;
			else
				OMatrix[i][j] = 0;
		
			if(OMatrix[i][j] != TMatrix[i][j])
			{
		
				wrong = 1;			/*if a pattern row's output was incorrect*/
			
				ChangeWeights(WMatrix[j], PMatrix[i], OMatrix[i][j], TMatrix[i][j], columns);

				ChangeBias(&BMatrix[0][j], OMatrix[i][j], TMatrix[i][j]);
			
			}
			if(test == 1)
				fprintf(test, "%g  ", TMatrix[i][j]);				 
				
		}
			
		if(test == 1)
			fprintf(test, "\n");
		
		if(wrong == 1)				/*at least one wrong output in the row*/
		{
			N_incorrect += 1;
			wrong = 0;
		}
	}
		
	if(test == 1)	/*if -t then exits after one epoch*/
	{
		fclose(test);
		exit(3);
	}

	if(N_incorrect == 0)
		Converge = 1;			/*The perceptron has converged*/
	
	printf("epoch %3d: patterns incorrect = %3d\n", epochCount, N_incorrect);

	epochCount += 1;
}


if(save_weights != 0)
	Save(swfile, WMatrix, BMatrix[0], rows, output); 

/*Freeing the dynamically allocated structures(Matrices)*/
Free(Matrices[0], rows);
Free(Matrices[1], rows);
Free(Matrices[2], output);
Free(Matrices[3], 1);
Free(Matrices[4], rows);
free(Matrices);
			

/* seed the random number generator */
srand48(seed);     

return 0;

}
