#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid1, pid2, pid3;

	/* fork a child process */
	pid1 = fork();
	//printf("this is pid1: %d", pid1);
	//printf("\n");
	/* fork another child process */
	pid2 = fork();
	//printf("this is pid2: %d",pid2);
	//printf("\n");
	/* and fork another */
	pid3 = fork();
	//printf("this is pid3: %d",pid3);
	printf("hello world\n");

	return 0;
}
