#include <signal.h>
#include <stdio.h>

void savePid(int pid, const char* fileName) {	
	FILE* f = fopen(fileName, "w+");
       	if (f) {
        	fprintf(f, "%d", pid);
                fclose(f);
	} else {
		printf("Couldn't open file %s\n", fileName);
	}
}

int main(int argc, char** argv) {
	pid_t chldPid = fork(); 
	int st = 0;
        const char* pidFile = argc > 1 ? argv[1] : "/home/box/pid"; 
	if (chldPid){
		savePid(chldPid, pidFile);
                printf("Waiting for child process %d...\n", chldPid);
		waitpid(chldPid, &st, 0);
                printf("Child process %d was finished with exit code %d\n", chldPid, st); 
	} else {
		printf("In child process\n");
        	signal(SIGTERM, SIG_IGN);
                signal(SIGINT,  SIG_IGN);     
		pause();
	}
	return 0;
}
