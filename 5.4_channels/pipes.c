#include <unistd.h>
#include <stdio.h>

void exec_process(const char* args) {
	//printf("execute process: %s\n", args);
	char* file = strtok(args, " ");
	if (file != NULL) {
		char* arg = strtok(NULL, " ");
		if (arg != NULL) {
			//printf("execlp(%s, %s, %s, NULL)\n", file, file, arg);
			execlp(file, file, arg, NULL);
		} else {
			//printf("execlp(%s, %s, NULL)\n", file, file);
			execlp(file, file, NULL);
		}
	}
}

void pipe_from_to(const char* from, const char* to) {
	//printf("pipe: %s -> %s\n", from==NULL ? "NULL" : from, to==NULL ? "NULL":to);
	int fd[2];
	if (pipe(fd) == -1) {
		printf("pipe create error\n");
		return;
	}
	if (fork()) {
	    if (from != NULL) {
	        close(STDOUT_FILENO);
		    dup2(fd[1], STDOUT_FILENO);
		    close(fd[1]);
		    close(fd[0]);
		    exec_process(from);
		}
	} else {	
	    if (to != NULL) {
	        close(STDIN_FILENO);
		    dup2(fd[0], STDIN_FILENO);
		    close(fd[1]);
		    close(fd[0]);
		    exec_process(to);
		}
	}
}


void parse_cmd(const char* cmd) {
	char* ch = strtok(cmd, "|");
	char* prev = NULL;
	while (ch != NULL) {		
	    prev = ch;
	    ch = strtok(NULL, "|");
        if (ch != NULL) {
            pipe_from_to(prev, ch);
        }
    }	
}


int main(int argc, char** argv) {
	if (argc < 1) {
		return 1;
	}
	parse_cmd(argv[1]);
	return 0;
}
