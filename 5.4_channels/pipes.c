#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


void execute_process(char* line) {
    //printf("line=%s\n", line);
	    
    while(isspace(*line)) line++;

    char* end = line + strlen(line) - 1;
    while(end > line && isspace(*end)) end--;
    *(end+1) = 0;
    
    char* args = strstr(line, " ");
    
    if (args != NULL) {
        
        char command[256];
        strncpy(command, line, args - line);
        command[args-line] = 0;
        
        while(isspace(*args)) args++;

        char* end = args + strlen(args) - 1;
        while(end > args && isspace(*end)) end--;
        *(end+1) = 0;
  
        //printf("command=%s, args=%s,\n", command, args);
        execlp(command, command, args, NULL);
    } else {
        //printf("command=%s\n", command);
        execlp(line, line, NULL);
    }
}

void parse_cmd(char* cmd, const char* file) {
	char* prev = NULL;
	char* next = NULL;

    int fd1[2];
	int fd2[2];
	int out = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	char* cur = strtok(cmd, "|");
	while (cur != NULL) {

	    next = strtok(NULL, "|");
	    pipe(fd2);
	    
	    //printf("cur=%s\n", cur);
	    //printf("next=%s\n", next);
	
	    if (fork() == 0) {
	        //child
	        if (prev) {
	            //stdin -> pipe fd[0]
	            close(STDIN_FILENO);
	            dup2(fd1[0], STDIN_FILENO);
	            close(fd1[0]);
	            close(fd1[1]);
	        }
	        if (next) {
	            //stdout -> pipe fd[1]
	            close(STDOUT_FILENO);
                dup2(fd2[1], STDOUT_FILENO);
                close(fd2[0]);
                close(fd2[1]);
            } else {
                //stdout -> file
                close(STDOUT_FILENO);
                dup2(out, STDOUT_FILENO);
                close(out);
            }
            execute_process(cur);
	        
	    } else {
	        //parent
	        if (prev) {
	            close(fd1[0]);
	            close(fd1[1]);
	        }
	        if (next) {
	            fd1[0] = fd2[0];
	            fd1[1] = fd2[1];
	        }
	    }
	    
	    prev = cur;
	    cur = next;
	}	
	    
	close(fd2[0]);
	close(fd2[1]);
    //close(fd1[0]);
	//close(fd1[1]);
    	
	while (wait(NULL) > 0);
}


int main(int argc, char** argv) {
	char buffer[1024];
    ssize_t size = read(STDIN_FILENO, buffer, 1024);
    if(size > 0) {
        buffer[size-1] = '\0';
        //printf("src command=%s\n", buffer);
        
	    parse_cmd(buffer, argv[1]);
	}
	return 0;
}
