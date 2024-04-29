#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_B = PTHREAD_MUTEX_INITIALIZER;

void *threadA_proc(void *data) {
  printf("thread A waiting get Resource A\n");
  pthread_mutex_lock(&mutex_A);
  printf("thread A got Resource A\n");
  
  sleep(1);
  
  printf("thread A waiting get Resource B\n");
  pthread_mutex_lock(&mutex_B);
  printf("thread A got Resource B\n");
  
  pthread_mutex_unlock(&mutex_B);
  pthread_mutex_unlock(&mutex_A);

  return (void *) 0;
}

void *threadB_proc(void *data) {
  printf("thread B waiting get Resource A\n");
  pthread_mutex_lock(&mutex_A);
  printf("thread B got Resource A\n");
  sleep(1);
  
  printf("thread B waiting get Resource B\n");
  pthread_mutex_lock(&mutex_B);
  printf("thread B got Resource B\n");

  pthread_mutex_unlock(&mutex_B);
  pthread_mutex_unlock(&mutex_A);

  return (void *) 0;
  
}

int main() {
  pthread_t tidA, tidB;
  
  pthread_create(&tidA, NULL, threadA_proc, NULL);
  pthread_create(&tidB, NULL, threadB_proc, NULL);
  
  pthread_join(tidA, NULL);
  pthread_join(tidB, NULL);
}
