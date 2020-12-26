#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SIZE	4096

const char *NAME = "/shm";

int main(int argc, char *argv[])
{
	pid_t pid;
	struct timeval current;

	pid = fork();

	if (pid<0)
	{
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid==0)
	{
		int fd;
		long *addr;
		char prefix[100] = "/bin/";
		strcat(prefix, argv[1]);

		char *const paralist[] = {prefix, NULL};
		gettimeofday(&current, NULL);
		long *sec = &current.tv_sec, *micro = &current.tv_usec;
		
		fd = shm_open(NAME, O_CREAT|O_RDWR, 0666);
		if (fd == -1){
			fprintf(stderr, "CHILD shm_open FAILED!\n");
			return 1;
		}

		ftruncate(fd, SIZE);
		addr = (long *)mmap(0, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		if (addr == MAP_FAILED){
                        fprintf(stderr, "CHILD mmap FAILED!\n");
                        return 1;
                }
	
		
		*addr = current.tv_sec;
		*(addr+1) = current.tv_usec;

		execv(prefix,paralist);
	}
	else
	{
		wait(NULL);

		int fd;
		long *addr;
		long second, microsecond, diff_sec, diff_microsec;
		
		gettimeofday(&current, NULL);
		fd = shm_open(NAME, O_RDONLY, 0666);
		if (fd == -1){
                        fprintf(stderr, "PARENT shm_open FAILED!\n");
                        return 1;
                }

		addr =(long *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);

		if (addr == MAP_FAILED){
			fprintf(stderr, "PARENT mmap FAILED!\n");
			return 1;
		}

		second = *addr;
		microsecond = *(addr+1);
		
		diff_sec = current.tv_sec - second;
		diff_microsec = current.tv_usec - microsecond;

		shm_unlink(NAME);

		printf("Elapsed time: %ld.%ld \n", diff_sec, diff_microsec);

	}

	return 0;

}
