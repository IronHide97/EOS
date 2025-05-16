#include <stdio.h>
#include <pthread.h>

#define MAX_LENGTH 1024
char  buff[MAX_LENGTH];

/* This read/write lock will be use to avoid race condition
    between these two threads */
pthread_rwlock_t  rwlock = PTHREAD_RWLOCK_INITIALIZER;

/*This thread function will be the producer of
  data in the buff */
/* Thread1 */  
void * producer (void * arg){

    /* When the producer is producing the data
    we dont want the consumer to come and read
    incompelete data which can cause undetermi-
    nistic behaviour of outr code */

    /* so to protect the above to happen we have
    read/write lock other than mutex lock. */

    /* pthread_rwlock_wrlock() is the function we use to
    put the write lock in the critical section, so if any 
    thread trying to read from this shared memory will get
    block untill this lock is not being unlocked by this
    thread.*/
    printf("ENTER upto 1024 char\n");

    pthread_rwlock_wrlock (&rwlock);

    scanf("%[^\n]s", buff);


    /* Now we will unlock the lock which will trigger the 
    unblocking of the threads wich were block due to this 
    write lock */
    pthread_rwlock_unlock(&rwlock);

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

    /* so to prevent the above we will have a read
    lock. Remember if we try to apply a read lock 
    on the lock object which is alread locked by
    another thread for writting purpose, this
    thread will get blocked.*/

    /* Also if we are into reading this lock will
    also prevent the producer to update the data 
    when we are still reading.*/

    /* pthread_rwlock_rdlock() is the function we use to
    put the read lock in the critical section, so if any 
    thread trying to write into this shared memory will get
    blocked untill this lock is not being unlocked by this
    thread.*/

    pthread_rwlock_rdlock(&rwlock);

    printf("%s\n", buff);

    pthread_rwlock_unlock(&rwlock);

    return NULL;
}


int main(){

    pthread_t thread1, thread2;

    pthread_create(&thread1, NULL, producer, NULL);
    pthread_create(&thread2, NULL, consumer, NULL);


    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_rwlock_destroy(&rwlock);
    return 0;
}
