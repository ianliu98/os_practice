#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void Collatz_conjecture(int);

int main(int argc, char *argv[])
{
	if (argc<2){
		fprintf(stderr, "Please input a number!\n");
		return -1;
	}
	if (argc>2){
		fprintf(stderr, "Too many inputs!\n");
		return -1;
	}
	int number = atoi(argv[1]);
	if (number <=0){
		fprintf(stderr, "A positive number is required!\n");
		return -1;
	}
	pid_t pid;
	pid = fork();
	
	if (pid<0){
		fprintf(stderr, "Fork faliled!\n");
		return -1;
	}
	
	if (pid==0){ /* child process */
		Collatz_conjecture(number);
		return 0;
	}
	else{
		wait(NULL);
		return 0;
	}
}

void Collatz_conjecture(int number)
{
	printf("The out put sequence is:\n");
	printf("%d,",number);
	while (number>1){
		if (number%2)
			number = 3*number+1;
		else
			number /= 2;
		printf("%d,",number);
	}
	printf("\n");
}
