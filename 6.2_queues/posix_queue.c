#include <fcntl.h>        
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

void handle_messages(union sigval sv){
	struct mq_attr attr;
	void* buf;
	mqd_t mqid = *((mqd_t *) sv.sival_ptr);
	
	if (mq_getattr(mqdes, &attr) == -1) {
		printf("getattr error\n");
		return;
	}
    buf = malloc(attr.mq_msgsize);
		   
	if (mq_receive(mqid, buf, buf_size, NULL) == -1) {
		printf("receive message error\n");
		return;
	}
	
	free(buf);
	
}

int main(int argc, char** argv){
	
	struct sigevent sev;
	const char* mqname = "/test.mq";
	
	mqd_t mqid = mq_open(mqname,  O_CREAT | O_RDWR, 0660, 0);
	if (mqid == -1){
		printf("error create queue %d\n", errno);
		return 1;
	}
	
	sev.sigev_notify = SIGEV_THREAD;
	sev.sigev_notify_function = handle_messages;
	sev.sigev_notify_attributes = NULL;
	sev.sigev_value.sival_ptr = &mqid;   /* Arg. to thread func. */
	
	if (mq_notify(mqid, &sev) == -1){
		printf("error notify %d\n", errno);
		return 1;
	}
		   
	mq_close(mqid);
	mq_unlink(mqname);
	
	int fd = open("/home/box/message.txt", O_CREAT | O_WRONLY, 0660);
	if (fd < 0) {
		printf("error open file\n");
		return 1;
	}
	write(fd, &msg.mtext, (ssize_t)rc);
	close(fd);
	
	return 0;
}