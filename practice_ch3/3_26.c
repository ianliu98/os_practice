#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define READ_END	0
#define WRITE_END	1
#define MAXLINE		1000

int main(int argc, char *argv[])
{
	if (argc < 2){
		fprintf(stderr, "Please input a string!\n");
		return -1;
	}
	if (argc > 2){
		fprintf(stderr, "Too many inputs!\n");
		return -1;
	}
	pid_t pid;
	int fd[2];
	if (pipe(fd) == -1){
		fprintf(stderr, "Pipe failed!\n");
		return -1;
	}

	pid = fork();
	if (pid < 0){
		fprintf(stderr, "Fork failed!\n");
		return -1;
	}
	if (pid == 0){ /* child process */
		close(fd[READ_END]);
		
		int length;
	        length = strlen(argv[1]) + 1;
		write(fd[WRITE_END], argv[1], length*sizeof(char));
		
		close(fd[WRITE_END]);

		return 0;
	}
	else{
		wait(NULL);
		close(fd[WRITE_END]);

		char string[MAXLINE], i=0;
		int length;
		length = strlen(argv[1]) + 1;
		read(fd[READ_END], string, length*sizeof(char));

		close(fd[READ_END]);
		
		while(string[i]!='\0'){
			if (string[i]>'a'&&string[i]<'z')
				string[i] -= ('a' - 'A');
			else if (string[i]>'A'&&string[i]<'Z')
				string[i] += ('a' - 'A');
			++i;
		}
		printf("%s\n", string);
		return 0;
	}
}
