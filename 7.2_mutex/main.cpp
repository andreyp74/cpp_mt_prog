#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_spinlock_t spin;
pthread_rwlock_t rwlock;

void save_pid() {
	pid_t pid = getpid();
	FILE* f = fopen("/home/box/main.pid", "w+");
	if (f) {
		fprintf(f, "%d", pid);
		fclose(f);
	}
}

void* thread_mutex(void*) {
    pthread_mutex_lock(&mutex);
}

void* thread_spin(void*) {
    pthread_spin_lock(&spin);
}

void* thread_rdlock(void*) {
    pthread_rwlock_rdlock(&rwlock);
}

void* thread_wrlock(void*) {
    pthread_rwlock_wrlock(&rwlock);
}

int main(int, char**) {
    save_pid();

    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_lock(&mutex);
    //pthread_mutex_unlock(&mutex);
    //pthread_mutex_destroy(&mutex);

    pthread_spin_init(&spin, PTHREAD_PROCESS_PRIVATE);
    pthread_spin_lock(&spin);
    //pthread_spin_unlock(&spin);
    //pthread_spin_destroy(&spin);
    
    pthread_rwlock_init(&rwlock, NULL);
    //pthread_rwlock_rdlock(&rwlock);
    pthread_rwlock_wrlock(&rwlock);
    //pthread_rwlock_unlock(&rwlock);
    //pthread_rwlock_destroy(&rwlock);
    
	pthread_t th_mutex_id;
	pthread_t th_spin_id;
	pthread_t th_rdlock_id;
	pthread_t th_wrlock_id;
	
	pthread_create(&th_mutex_id, NULL, thread_mutex, NULL);
	pthread_create(&th_spin_id, NULL, thread_spin, NULL);
	pthread_create(&th_rdlock_id, NULL, thread_rdlock, NULL);
	pthread_create(&th_wrlock_id, NULL, thread_wrlock, NULL);

	pthread_join(th_mutex_id, NULL);
	pthread_join(th_spin_id, NULL);
	pthread_join(th_rdlock_id, NULL);
	pthread_join(th_wrlock_id, NULL);

    return 0;
}
