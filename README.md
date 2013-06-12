Implements a two layer sigmoidal neural network learning system with backpropagation

Reads in a file of patterns. The format of the pattern
file is:

	Npatterns
	input_size
	output_size
	ipattern1 opattern1
	ipattern2 opattern2
	...
	ipatternN opatternN

The network will configure itself based on the pattern
length and the hidden layer size.  Note that the hidden
layer size is not given above.  This should be another
command line parameter to the system in the same manner as
the previous assignment, using the flag "-h".  This
parameter is thus *required*.  

Train the network for a specified number of epochs, or
until convergence.  During this process, the system will
print out the root mean squared error after every pass
through the patterns (one epoch), as in the following
example :

ieng9.ucsd.edu[13] bp -p xor.pat -h 2 -n 5
epoch   1 :  RMSE = 1.07067
epoch   2 :  RMSE = 1.06953
epoch   3 :  RMSE = 1.06878
epoch   4 :  RMSE = 1.06833
epoch   5 :  RMSE = 1.06808

The root mean squared error is an error measure based on the 
sum squared error, except that it does not vary with the number 
of patterns in the training set.  It also enjoys the property 
that if your network has just learned the average teaching signal
(ignoring the input altogether), it is 1.0, and if your network 
has learned the patterns perfectly, it is 0.0.  Here it is:

        SUM(all pats (p)) SUM(all outputs (i)) (Teacher_pi - output_pi)^2
RMSE =    -----------------------------------------------------------
       SUM(all pats (p)) SUM(all outputs (i)) (Teacher_pi - avg_Teacher_i)^2

avg_Teacher_i = [SUM(all pats (p)) Teacher_pi]/NPATS

So the denominator above is constant, and can be computed by
a couple of passes through the training set, once to get the 
average, then a second time to get the sum.

Example:
		pats: 1 2 3 4 5 6 7 8	avg_teacher
---------------------------------------------------
teacher for output 1: 1 1 1 1 0 0 0 0       0.50
teacher for output 2: 1 1 1 1 1 1 0 0       0.75


RMSE Denominator for above example : 

	4 * (1 - .50)^2  + 
	4 * (0 - .50)^2  +
	6 * (1 - .75)^2  + 
	2 * (0 - .75)^2  	= 3.5

Gives you a way of saving weights and reading them back in, 
and observing the behavior of the network with those weights.
It will save with the weights the input, HIDDEN, and output
size, so that the simulator should be able to check whether a
set of weights makes sense when given a pattern file.  To make
sure we can share weights as well, the format of the weight 
file should be:

	input_size
	hidden_size
	output_size
	bias[1..hidden+output_size]
	input->hidden weight_matrix row 0
	input->hidden weight matrix row 1
	...
	input->hidden weight matrix row hidden_size - 1
	hidden->output weight_matrix row 0
	hidden->output weight matrix row 1
	...
	hidden->output weight matrix row output_size - 1

It demonstrate empirically that it can learn AND,
OR, NOT and XOR.


#Notes on Implementing Random Pattern Presentation

For some problems, presenting the patterns in the training set always
in the same order during training can slow learning and lead to an
increase in local minima.  To get around this problem, we will add an
option, -R, to our simulators that causes the order in which patterns
are presented for training to be re-randomized on every epoch.  Note
that we are making this a command line option rather than a built-in
feature.  This is because random pattern presentation order does not
always help to speed learning -- for example, it tends to make learning
more difficult on the xor problem.

#Command line options

	-h <integer>		The number of hidden units.
				Should be >= 1. A *required*
				parameter.

	-p <file name>		name of file containing training
				patterns -- your program can
				expect that this argument will
				always be provided.

	-l <file name>		name of file containing weights
			 	to initialize the network with
				-- if this option is not invoked
				then you should randomly initialize
				the weights in your network 

	-r <float wrange>	range of initial weights. They
				should be initialized to random
				values in the range -wrange..+wrange

	-s <file name>		name of file to save weights
			 	to when network training is 
				completed -- if this option is not
				invoked, do not save the weights

	-S <file name>		name of file containing a long
				integer to be used as the random
				number generator seed, OR the name
				a file that does NOT exist and into
				which the seed generated by the
				program is to be saved

	-n <integer> 		maxinum number of training epochs; 
				if left unspecified, this should default
				to 1000

	-R 	 		use of this option will cause pattern 
				presentation order to be re-randomized
				on every epoch of training;  note that, 
				since pattern randomization is only specified 
				for training mode, this flag can be ignored 
				if used with the -t flag

	-t <file name>		this is the 'test' option -- when
				invoked, a record of the network's output 
				activation for each pattern in the training 
				set should be saved to <file name> 
				(no learning should be performed when the 
				program is running in this mode);  the 
				program should exit after completing one 
				sweep through the pattern file 

	-e <float>		the learning rate, default 0.1

	-m <float>		momentum parameter, default 0.9

	-c <float>		error criterion: an amount
				to use to stop training when
				the rmse (defined above) is
				below this number. A typical
				value might be 0.04 (make this
				the default).


