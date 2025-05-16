#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define MAX_LENGTH 1024
char  buff[MAX_LENGTH];

/* This semaphore will be use to avoid race condition
    between these two threads */
sem_t sem;

/*This thread function will be the producer of
  data in the buff */
/* Thread1 */  
void * producer (void * arg){

    /* When the producer is producing the data
    we dont want the consumer to come and read
    incompelete data which can cause undetermi-
    nistic behaviour of outr code */

    /* so to protect the above to happen we have
    thread semaphores. So before entring critical
    section we should take the semaphore do access
    the critical reagion and give the semaphore
    
    wait   ->> take
    signal ->> give
    */

    
    printf("ENTER upto 1024 char\n");

    
    /* Take the semaphore before entering 
    the critical section, any thread trying
    to enter critical setion when the sem
    is already taken will get blocked */    
    sem_wait(&sem);

    
    scanf("%[^\n]s", buff);

    /* Give the semaphore after accessing 
    the critical section, any thread blocked
    trying to enter critical section now will
    get unblocked */

    sem_post(&sem);

    return NULL;
}

/*This thread function will be the consumer of 
    data from buff */
/* Thread2 */
void * consumer (void * arg){

    /* We dont want to read an incomplete data 
    which is still in process of getting update
    as this can cause undeterministic behaviour
    of the code */

    /* Take the semaphore before entering 
    the critical section, any thread trying
    to enter critical setion when the sem
    is already taken will get blocked */  

    sem_wait(&sem);

    printf("%s\n", buff);

    /* Give the semaphore after accessing 
    the critical section, any thread blocked
    trying to enter critical section now will
    get unblocked */

    sem_post(&sem);

    return NULL;
}


int main(){

    pthread_t thread1, thread2;
    
    sem_init(&sem, 0, 1);
    pthread_create(&thread1, NULL, producer, NULL);
    pthread_create(&thread2, NULL, consumer, NULL);


    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sem_destroy(&sem)
    return 0;
}
