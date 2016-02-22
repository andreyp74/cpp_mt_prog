#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void save_pid() {
	pid_t pid = getpid();
	FILE* f = fopen("/home/box/main.pid", "w+");
	if (f) {
		fprintf(f, "%d", pid);
		fclose(f);
	}
}

void* inc(void* pval) {
	int* pi = (int*) pval;
	++(*pi);

	return pi;
}


int main(int, char**) {
	save_pid();

	pthread_t thread_id;
	int val = 1;
	pthread_create(&thread_id, NULL, inc, &val);
	void* res;
	pthread_join(&thread_id, res);
	free(res);

	return 0;
}
