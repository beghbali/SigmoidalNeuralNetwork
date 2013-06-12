# Sample Makefile for C language programs

CC = cc

nn: source/runner.c source/next_rand_index.c
	$(CC) -o bp  source/runner.c source/next_rand_index.c -lm

clean:
	rm *.o

