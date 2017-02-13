#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define PAGE_SIZE 4096 /* Where is this defined */
#define NUM_IO 10

int main(int argc, char *argv[])
{
	int io, fd, ret;
	off_t size;
	ssize_t wrote;
	struct stat buf;
	char wdata[PAGE_SIZE];

	/* Input argument check */

	if ( argc!=2 ){
		fprintf(stderr, "error: %s requires one argument.\n",
			argv[0]);
		return -1;
	}

	/* Open the requested file */

	fd = open(argv[1], O_RDWR, O_DIRECT | O_SYNC);
	if ( fd == -1 ){
		perror("open");
		return -1;
	}

	/* Check the file opened is a block device */

	ret = fstat(fd, &buf);
	if ( ret != 0 ){
		perror("fstat");
		return -1;
	}
	if ( !S_ISBLK(buf.st_mode) ){
		fprintf(stderr, "error: %s is not a block device file.\n",
			argv[1]);
		return -1;
	}

	/* Get the size of the device. */

	size = lseek(fd, 0, SEEK_END);
	if ( size == -1 ){
		perror("fstat");
		return -1;
	}
	fprintf(stdout,"The size of %s appears to be %zd B.\n",
		argv[1], size);

	/* Perform some PAGE_SIZE byte random IO on the device. */
	for ( io=0; io<NUM_IO; io++ ){
		memset(wdata, io, 4096);
		lseek(fd, io*PAGE_SIZE, SEEK_SET);
		wrote = write(fd, wdata, PAGE_SIZE);
		if ( wrote == -1 ){
			perror("write");
			return -1;
		}
		sync();
		fprintf(stdout, "Performed write %2d/%2d (%d/%d)\r",
			io+1, NUM_IO, io, io*PAGE_SIZE);
	}
	fprintf(stdout,"\n");
		

	close(fd);
	return 0;
}
			
