#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <fcntl.h>

struct message{
	long type;
	char mtext[80];
} msg;

int main() {
	int rc = 0;
	key_t key = ftok("/tmp/msg.temp", 1);
	int msgid = msgget(key, IPC_CREAT);
	if (msgid < 0) {
		printf("error queue create\n");
		return 1;
	}
	
	rc = msgrcv(msgid, &msg, sizeof(msg.mtext), 0, 0);
	if (rc < 0) {
		printf("msgrcv failed, rc=%d\n", rc);	
		return 1;
	}	
	int fd = open("/home/box/message.txt", O_CREAT | O_WRONLY, 0666);
	if (fd < 0) {
		printf("error open file\n");
		return 1;
	}
	write(fd, &msg.mtext, (ssize_t)rc);
	close(fd);

	msgctl(msgid, IPC_RMID, NULL);
	
	return 0;
}
