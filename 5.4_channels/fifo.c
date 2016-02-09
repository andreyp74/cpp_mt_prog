#include <sys/types.h>
//#include <sys/stat.h>
//#include <stdio.h>
//#include <unistd.h>
#include <fcntl.h>

#define MAX_BUF 4096

int main(int argc, char** argv) {
 	int ifd, ofd, rbytes;
	char buf[MAX_BUF];
	if (argc < 2) {
		//printf("usage fifo in out\n");
		return -1;
	}
	mkfifo(argv[1], 0666);
	mkfifo(argv[2], 0666);
	
	ifd = open(argv[1], O_RDONLY);
	ofd = open(argv[2], O_WRONLY);
	while (rbytes = read(ifd, buf, MAX_BUF)) {
		write(ofd, buf, rbytes);
	}

	close(ifd);
	close(ofd);

	unlink(argv[1]);
	unlink(argv[2]);

	return 0;
}
