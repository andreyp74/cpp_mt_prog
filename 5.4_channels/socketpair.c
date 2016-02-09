#include <sys/socket.h>
#include <stdio.h>

#define MAX_BUF 4096

int main(int argc, char** argv) {
	int fd[2];
	int rbytes;
	char buf[MAX_BUF];
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fd) < 0) {
		printf("socketpair error/n");
		return -1;	
	}

	if (fork()) {
		close(fd[1]);
		while (rbytes = read(fd[0], buf, MAX_BUF)) {
			buf[rbytes] = 0;
			printf("recv: %s", buf);
		}
	} else {
		close(fd[0]);
		while(1) {
			write(fd[1], "test message", 13);
		}
	}
	return 0;
}
