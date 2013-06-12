
#include <stdlib.h>
#include <sys/time.h>


int next_random_index(int reset, int limit)
/* after initialization, successive calls made to this       */
/* function will return each integer in a random permutation */
/* of the sequence of integers in the interval [0, limit).   */
/* integers in the sequence are returned one at a time until */
/* all integers in the sequence have been returned exactly   */
/* once.  after all integers in the sequence have been       */
/* returned, this function will return -1 until it is reset. */
/* this function is initialized or reset by calling it with  */
/* 'reset'=1 and 'limit' set to the appropriate upper bound  */
/* for the desired sequence.  subsequent calls made with     */
/* 'reset'=0 and 'limit' set to the same value as on the     */
/* initializing/resetting call return the elements in the    */
/* permuted sequence until they are exhausted.               */
/*                                                           */
/* note :                                                    */
/* assumes srand48() has already been called elsewhere to    */
/* initialize drand48().                                     */
{ 
int        i;	/* generic loop variable */
int	 old;	/* temp for swapping values in the sequence */
int	npat;	/* temp for holding the index of an elt to be swapped */

static int nextindex;	/* the index of the next element of the */
			/* permuted sequence to be returned     */

static int *rand_index; /* the array that holds the permuted sequence */

if(reset)
/* initialize the random index array */
   {
   free(rand_index);
   rand_index = (int*) malloc(sizeof(int)*limit);
   for (i = 0; i < limit; i++)
            rand_index[i] = i;

   /* this loops actually randomizes the elements of
      rand_index */

   for (i = 0; i < limit; i++) {
     /* drand48 gives a random number between 0 and 1 */
      npat = drand48() * (limit - i) + i;
      old = rand_index[i];
      rand_index[i] = rand_index[npat];
      rand_index[npat] = old;
    }

   nextindex = 0;
   return 1;
   }
else
   if(nextindex<limit)
      return rand_index[nextindex++];
   else
      return -1;

}
