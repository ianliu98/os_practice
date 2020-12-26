#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define READ_END	0
#define WRITE_END	1

int main(int argc, char *argv[])
{
	if (argc != 3){
		fprintf(stderr, "please check number of inputs!\n");
		return -1;
	}

	int fd[2];
	if (pipe(fd)==-1){
		fprintf(stderr, "Pipe Failed!\n");
		return -1;
	}

	pid_t pid;
	pid = fork();
	if (pid < 0){
		fprintf(stderr, "Fork Failed!\n");
		return -1;
	}
	else if (pid == 0) { /* child process */
		struct stat sb;
		stat(argv[1], &sb);

		int size, fd_src;
	        size = (int)sb.st_size;
		char buffer[size+1];
		fd_src = open(argv[1], O_RDONLY);
		read(fd_src, buffer, size+1);
			
		close(fd_src);
		close(fd[READ_END]);
		write(fd[WRITE_END], buffer, size+1);
		close(fd[WRITE_END]);

		return 0;
	}
	else {
		wait(NULL);
		close(fd[WRITE_END]);

		char buffer[5000];	// how to decide this size??
		read(fd[READ_END], buffer, 5000);
	
		close(READ_END);

		int length = strlen(buffer);
		int fd_det;
		fd_det = open(argv[2], O_RDWR|O_CREAT);
		write(fd_det, buffer, length);
		close(fd_det);

		return 0;
	}
}
