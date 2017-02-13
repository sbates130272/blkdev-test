#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	int fd, ret;
	off_t size;
	struct stat buf;

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
		

	close(fd);
	return 0;
}
			
