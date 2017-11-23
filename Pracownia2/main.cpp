#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <ctime>
#include <unistd.h>

int Hunters, Cooks, Animals, Food;
pthread_mutex_t animalMutex, foodMutex;

void *hunterJob(void *id) {
   for(int i = 0; i < 365; i++) {
    while(pthread_mutex_lock(&animalMutex)) {}
    int v1 = rand() % 6;
    int v2 = rand() % 6;
    if(v1 > v2) {
      Animals++;
    }
    pthread_mutex_unlock(&animalMutex);
    while(pthread_mutex_lock(&foodMutex)) {}
    if(Food > 0) {
      Food--;
      pthread_mutex_unlock(&foodMutex);
    } else {
      pthread_mutex_unlock(&foodMutex);
      pthread_exit(NULL);
    }
    usleep(10000);
   }
   return 0;
}
  
void *cookJob(void *id) {
   for(int i = 0; i < 365; i++) {
    while(pthread_mutex_lock(&animalMutex)) {}
    if(Animals > 0) {
      Animals--;
      pthread_mutex_unlock(&animalMutex);
      while(pthread_mutex_lock(&foodMutex)) {}
      int v1 = rand() % 6;
      Food += v1;
      pthread_mutex_unlock(&foodMutex);
    } else {
      pthread_mutex_unlock(&animalMutex);
    }
    while(pthread_mutex_lock(&foodMutex)) {}
    if(Food > 0){
      Food--;
      pthread_mutex_unlock(&foodMutex);
    } else {
      pthread_mutex_unlock(&foodMutex);
      pthread_exit(NULL);
    }
    usleep(10000);
  }
  return 0;
}

int main (int argc, char *argv[]) {
  if(argc != 5){
    exit(-1);
  }
  srand (time(NULL));
  int startHunters = atoi(argv[1]), startCooks = atoi(argv[2]);
  Animals = atoi(argv[3]);
  Food = atoi(argv[4]);

  pthread_t huntersThreads[startHunters];
  pthread_t cooksThreads[startCooks];

  int rc;
   
  for(long i = 0; i < startHunters; i++) {
    rc = pthread_create(&huntersThreads[i], NULL, hunterJob, NULL);
    if (rc) {
      std::cout << "Error:unable to create thread," << rc << '\n';
      exit(-1);
    }
  }

  for(long i = 0; i < startCooks; i++) {
    rc = pthread_create(&cooksThreads[i], NULL, cookJob, NULL);
    if (rc) {
      std::cout << "Error:unable to create thread," << rc << '\n';
      exit(-1);
    }
  }

  pthread_exit(NULL);
}
