#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#define SIZE 5

int nums[SIZE] = {0,1,2,3,4};

int main()
{
	int i;
	pid_t pid;
	pid = fork();

	if (pid == 0) {
		for (i = 0; i < SIZE; i++) {
			nums[i] *= -i;
			printf("CHILD: %d ",nums[i]); /* LINE X */
		}
		printf("\n");
		return 0;
	}
	else if (pid > 0) {
		sleep(20);	// child process has already exited, yet parent process is sleeping, child cannot return
		for (i = 0; i < SIZE; i++)
			printf("PARENT: %d ",nums[i]); /* LINE Y */
		printf("\n");
	}

	return 0;
}
