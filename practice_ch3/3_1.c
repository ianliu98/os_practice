#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int value = 5;

int main()
{
	pid_t pid;

	printf("%d", pid);
	printf("\n");
	pid = fork();

	printf("%d ", pid);
	printf("\n");
	if (pid ==0) { /* child process */
		value += 15;
		printf("CHILD: value = %d\n", value);
		return 0;
	}
	else if (pid > 0) { /* parent process */
		wait(NULL);
		printf("PARENT: value = %d  %d\n", value, pid); /* LINE A */
		return 0;
	}
}
