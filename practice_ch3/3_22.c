#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

char *name = "Collatz";
int size = 1000;

int main(int argc, char *argv[])
{
	if (argc < 2){
		fprintf(stderr, "Please input a positve number! \n");
		return -1;
	}
	if (argc > 2){
		fprintf(stderr, "Too many inputs! \n");
		return -1;
	}
	int number = atoi(argv[1]);
	if (number <=0){
		fprintf(stderr, "A positive number is necessary! \n");
		return -1;
	}

	pid_t pid;
	pid = fork();
	if (pid < 0){
		fprintf(stderr, "Fork failed!\n");
		return -1;
	}

	if (pid == 0){
		int fd;
		fd = shm_open(name, O_RDWR|O_CREAT, 0666);
		ftruncate(fd, size);

		/* Collatz conjecture */
		int seq_rl[size], n=0, k, number_ori = number;
		while (number>1){
			if (number%2)
				number = 3*number + 1;
			else
				number /= 2;
			seq_rl[n] = number;
			++n;
		}

		int *pointer = (int *)mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

		*pointer = n;
		*(pointer+1) = number_ori;
		for (k=0; k<n; ++k)
			pointer[k+2] = seq_rl[k];
		
		munmap(0, size);
		return 0;
	}
	else{
		wait(NULL);
		int fd;
		fd = shm_open(name, O_RDONLY, 0666);

		int *pointer = (int *)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);

		int n = pointer[0], i;
		printf("The sequence will be: \n");
		for (i=0; i<=n; ++i)
			printf("%d ", pointer[i+1]);
		printf("\n");	
		shm_unlink(name);

		return 0;
	}
}
