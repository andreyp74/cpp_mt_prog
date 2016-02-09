#include <unistd.h>
#include <stdio.h>

void exec_process(const char* args) {
	printf("execute process: %s\n", args);
	char* file = strtok(args, " ");
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

void process(int* fd, const char* cmd, const char* prev, const char* next) {
	int fd1[2];
	if (pipe(fd1) < 0) {
		return NULL;
	}
	if (!fork()) {
	    if (prev != NULL) {
	        close(STDIN_FILENO);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
	    }
            if (next != NULL) {
	    	close(STDOUT_FILENO);
            	dup2(fd1[1], STDOUT_FILENO);
            	close(fd1[1]);
            	close(fd1[0]);
	    }
            dup2(fd[0], fd1[0]);
	    exec_process(cmd);
	}
}


void parse_cmd(const char* cmd) {
	char* ch = strtok(cmd, "|");
	char* prev = NULL;
	int fd[2];
	while (ch != NULL) {
		char* next = strtok(NULL, "|");
		process(fd, ch, prev, next);
    		prev = ch;
		ch = next;
	}	
}


int main(int argc, char** argv) {
	if (argc < 1) {
		return 1;
	}
	parse_cmd(argv[1]);
	return 0;
}
