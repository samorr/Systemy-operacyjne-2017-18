#include <iostream>
#include <cstdlib>
#include <pthread.h>

void *printChildMessage(void *id) {
   long m = (long) id;
   std::cout << "hello, i'm child thread number " << m << "!\n";
   pthread_exit(NULL);
}

int main () {
  pthread_t threads[5];
  int rc;
  void *status;
   
  for(long i = 0; i < 5; i++) {
    rc = pthread_create(&threads[i], NULL, printChildMessage, (void *)i);
    if (rc) {
      std::cout << "Error:unable to create thread," << rc << '\n';
      exit(-1);
    }
  }

  for(long i = 0; i < 5; i++) {
    rc = pthread_join(threads[i], &status);
    if (rc) {
      std::cout << "Error:unable to join," << rc << '\n';
      exit(-1);
    }
  }
  std::cout << "hello, i'm parent thread!\n";
  pthread_exit(NULL);
}
