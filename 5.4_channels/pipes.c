#include <unistd.h>
#include <stdio.h>

void dup_and_close(int from, int to) {
	close(to);
	dup2(from, to);
	close(from);
}

void create_pipe(const char* from, const char* to) {
	printf("pipe: %s -> %s\n", from==NULL ? "NULL" : from, to==NULL ? "NULL":to);
	if (to == NULL) {
		return;
	}
	int fd[2];
	if (pipe(fd) == -1) {
		printf("pipe create error\n");
		return;
	}
	if (!fork()) {
		dup_and_close(fd[0], STDIN_FILENO);
		exec_process(to);
	} else if (from != NULL) {
		dup_and_close(fd[1], STDOUT_FILENO);
	}
}

void exec_process(const char* args) {
	printf("parse args: %s\n", args);
	char* file;
	file = strtok(args, " ");
	if (file != NULL) {
		char* arg = strtok(NULL, " ");
		if (arg != NULL) {
			printf("execlp(%s, %s, %s, NULL)\n", file, file, arg);
			execlp(file, file, arg, NULL);
		} else {
			printf("execlp(%s, %s, NULL)\n", file, file);
			execlp(file, file, NULL);
		}
	}
}


void parse_cmd(const char* cmd) {
	char* ch = strtok(cmd, "|");
	char* prev = NULL;
	while (ch != NULL) {		
		create_pipe(prev, ch);	
		prev = ch;
		ch = strtok(NULL, "|");
	}
}


int main(int argc, char** argv) {
	if (argc < 1) {
		return 1;
	}
	printf("try to run cmd: %s\n", argv[1]);
	parse_cmd(argv[1]);
	return 0;
}
