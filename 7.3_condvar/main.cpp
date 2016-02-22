#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_barrier_t barrier;

void save_pid() {
	pid_t pid = getpid();
	FILE* f = fopen("/home/box/main.pid", "w+");
	if (f) {
		fprintf(f, "%d", pid);
		fclose(f);
	}
}

void* thread_cond(void*) {
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
}

void* thread_barr(void*) {
    pthread_barrier_wait(&barrier);
}


int main(int, char**) {
    save_pid();

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    
    pthread_barrier_init(&barrier, NULL, 2);
    //pthread_barrier_destroy(&barrier, NULL);
    
    pthread_mutex_lock(&mutex);
    //pthread_cond_signal(); 
    //pthread_mutex_unlock(&mutex);
    
    //pthread_mutex_destroy(&mutex);
    //pthread_cond_destroy(&cond);
    
	pthread_t th_cond_id;
	pthread_t th_barr_id;
	
	pthread_create(&th_cond_id, NULL, thread_cond, NULL);
	pthread_create(&th_barr_id, NULL, thread_barr, NULL);

	pthread_join(th_cond_id, NULL);
	pthread_join(th_barr_id, NULL);

    return 0;
}
