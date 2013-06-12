#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream.h>



int main(int argc, char **argv)
{

extern	char *optarg;

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
double  wrange = 0.1;   /* random initial weights are set to values  */
                        /* between -wrange and +wrange               */

/* seed for random number generator */
long	seed = ((time(NULL)&0x0000ffff) | (getpid()<<16)); 


FILE	*cmdfp;
FILE	*seedfp;


while ((c = getopt(argc, argv, "p:l:s:n:t:r:S:")) != EOF) {
	switch (c) {
		case 'p':
			strcpy(patfile,optarg);
			break;
		case 'l':
			strcpy(iwfile,optarg);
			break;
		case 's':
			strcpy(swfile,optarg);
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

/* seed the random number generator */
srand48(seed);


}
