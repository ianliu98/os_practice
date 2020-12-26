#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SIZE	4096
#define READ_END	0
#define WRITE_END	1

const char *NAME = "/shm";

int main(int argc, char *argv[])
{
	pid_t pid;
	struct timeval current;
	int fd[2];

	if(pipe(fd) == -1){
		fprintf(stderr, "Pipe Failed!");
		return 1;
	}

	pid = fork();
	if (pid<0)
	{
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid==0)
	{
		close(fd[READ_END]);

		char prefix[100] = "/bin/";
		strcat(prefix, argv[1]);
		char *const paralist[] = {prefix, NULL};

		gettimeofday(&current, NULL);
		long current_time[2] = {current.tv_sec, current.tv_usec};
		
		write(fd[WRITE_END], current_time, sizeof(long)*2);
		
		close(fd[WRITE_END]);

		execv(prefix,paralist);
	}
	else
	{
		wait(NULL);
		close(fd[WRITE_END]);

		long current_time[2], diff_sec, diff_microsec;
		
		gettimeofday(&current, NULL);
		
		read(fd[READ_END], current_time, sizeof(long)*2);
		
		diff_sec = current.tv_sec - current_time[0];
		diff_microsec = current.tv_usec - current_time[1];

		printf("Elapsed time: %ld.%ld \n", diff_sec, diff_microsec);

		close(fd[READ_END]);

	}

	return 0;

}
