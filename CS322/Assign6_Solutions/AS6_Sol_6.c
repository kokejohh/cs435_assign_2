#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex_var; 
pthread_cond_t cv; 

int count = 0; 

void * foo(void * arg){

   pthread_mutex_lock(&mutex_var);  
   printf("foo() critical 1: block on count == %d\n"
                                        , count); 
   while(count <= 0){
     pthread_cond_wait(&cv, &mutex_var); 
   }
   printf("foo() critical 2: new count is %d\n"
                                        , count); 
   count--;
   printf("foo() critical 2: update count to %d\n"
                                        , count); 
   pthread_mutex_unlock(&mutex_var);  

   pthread_exit((void *)NULL); 
}

void * bar(void * arg){
   //sleep(1);
   pthread_mutex_lock(&mutex_var);  
   count ++;
   printf("bar() update count to %d\n", count); 
   pthread_cond_signal(&cv);
   pthread_mutex_unlock(&mutex_var);  

   pthread_exit((void *)NULL); 
}

#define NUM_THREADS 4

int main(void){

   pthread_t footid[NUM_THREADS];
   pthread_t bartid[NUM_THREADS];
   void *tret; 
   int i; 
    
   pthread_mutex_init(&mutex_var, NULL); 
   pthread_cond_init(&cv, NULL); 

   for(i=0; i < NUM_THREADS; i++){
     pthread_create(&footid[i], NULL, foo, NULL);  
     pthread_create(&bartid[i], NULL, bar, NULL);  
   }

   for(i=0; i < NUM_THREADS; i++){
     pthread_join(footid[i], NULL); 
     pthread_join(bartid[i], NULL); 
   }

   pthread_cond_destroy(&cv); 
   pthread_mutex_destroy(&mutex_var); 
   printf("main() count is %d\n", count); 
}
