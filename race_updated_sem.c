#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include <sys/sem.h>
#include<sys/shm.h>


//we need to use unique KEY values, for creating
//a shared-memory IPC mechanism instance and
//a semaphore IPC mechanism instance
//
//
#define KEY1 1555   //this will be unique, in the range of keys of 
                    //shared-memory IPC 
#define KEY2 1556   //this will be unique, in the range of keys of 
                    //semaphore IPC 

//explicit shared data, which will be part 
//of a shared-data segment of processes -
//this shared-data object will be placed, in a
//shared-data segment
//we will be using pointer techniques to place
//this object, in a shared memory segment
//
// 
struct shmarea {

  unsigned long count;
  unsigned long count1;
  unsigned long count2;
  unsigned long count3;
  unsigned long count4;

}; 

//this pointer-variable is related to our shared-memory 
//object above 
//
struct shmarea *shma; //we will store a shared-data segment's
                      //starting virtual-address, in this 
                      //variable, after a shared-memory 
                      //set-up  ??

//
//one of the semaphore system call APIs uses this 
//union structure - this is used by semctl() - see
//the code below ??
//
//certain field of this union is used, for certain
//options of semctl()
//
//man semctl
//
union semun { //used with semctl() system call for initializing
              //getting the current semaphore count values
  int val;    //we will be using val field of an union, 
              //for SETVAL-command 
              
  unsigned short *array; //we will be using array-field of 
                         //the union, for SETALL-command
                 
                         // other fields are omitted, since
                         // we are not using 

};  




int main()
{
  //
  //be warned - if we re-use return variables, in 
  //            certain places, program logic will break
  //
  //check you code for return variable re-usage and
  //program logic
  //
   int ret,ret1,ret2,status,id,sem_id;
  
  //struct sembuf{} is used as data-type, in a parameter
  //of semop() 
  //refer to code below 
  //refer to man semop() 
  //
   struct sembuf sembuf1; //sembuf1 is an user-space variable, 
                          //that is used to pass parmeters to 
                          //semop() - do not mix it with 
                          //          system-space semaphore-object 
   union semun u1; //an instance this union is passed to 
                   //semctl

   unsigned long int i=0, j=0;//these iteration variables
                              //used

     //p1 is used to pass an unique KEY value - in this case, 
     //we are using a hard-coded KEY value - if you want to 
     //know more rules, refer to man shmget
     //
     //p2 will be the shared-memory IPC's shared memory 
     //segment size - as per rules, must be a multiple of 
     //page-size ---> 4096/8192/....  
     //
     //getpagesize() will provide the page-size, in a 
     //given system...
     //
     id =  shmget(KEY1, 2*2*getpagesize(), IPC_CREAT|0600);
     //in the above shmget(), p3 is used to set the 
     //multi-user access permissions, for this shared-memory 
     //object/segment - 0600 is the minimum permissions, for
     //the current user/owner of this shared-memory 
     //objecti - owner is the user-id -- meaning, all processes 
     //of this owner/user-id are allowed to access this 
     //shared-memory object and its segment(s) - if needed, 
     //we can change it to 0660 or 0666, for granting 
     //permissions to other user-ids /processes
     //if we need to create a new shared-memory objecti, 
     //we must use IPC_CREAT - IPC_CREAT will create a 
     //new shared-memory objecti, if already not created-
     //otherwise, will just return the handlei/idi of the
     //existing shared-memory objecti for the given KEYi value
     // 
     //refer to man shmget(), for more details 
     //id provides handle to a shared-memory object 
     //associated, with this KEY value 
 

   //comments for shmat(p1, p2, p3);
   //--->p1 will be the idi of a shmobjecti 
   //param2 - 0 means, system allocates shared, virtual-segment addresses,along with
   //a shared, VADk !!! you may use non-zero values, but
   //param3 - flags are initially set to 0 !!!
   //we may use flags to set certain specific access permissions !!!
   shma = shmat(id,0,0); 
   //
   //
   //check for error ??
   //
   if(shma == (-1)) { perror("error in shmat"); exit(2); }


   //if successful, shma holds a valid vai , which will be the first 
   //virtual-addressi, in the shared, virtual-segmenti created , by 
   //shmat().......we will be using this pointer-variable, 
   //to access shared-memory data-segmenti...refer to the code, 
   //after semaphore set-up...
   // 
   //
   // 
   //step 1.semget() is used to create a new semaphore objecti/
   //p1 must be an integer value/an unique KEY value, for 
   //every semaphore-objecti  
   //p2 is 1, so that, a single-semaphorei(in a semaphore-objecti)
   //is created, in this semaphore-objecti
   //
   //p3 -->IPC_CREAT is to request creation of
   //a new semaphore object, if already not 
   //existing - if existing, just give me 
   //the handle - refer to man 2 semget - 
   //0600 is for multi-user access permissions
   //to this semaphore objecti /semaphorei -  
   //for now, the owner of this semaphore is 
   //the only one allowed to access this semaphore - 
   //the current user-account is the owner of this semaphore 
   //objecti -  
   //there is another, long-story behind this ??
   //
   //let us use a standard set of flags, for p3
   sem_id = semget(KEY2,1,IPC_CREAT|0600); 
   if(sem_id <0) { perror("error in creating a semaphore"); exit(2); }
   //return value cannot be <0 - if <0, it is an error
   //--->id is >=0 - you can print and understand more...
   //
   //
   //step2. we need to initialize a semaphorei of a 
   //--->there is no default value, for a semaphorei, in 
   //    a semaphore-objecti
   //--->in this code-sample, we need a binary-semaphore, 
   //    so the initial value will be set to 1
   //
   u1.val = 1 ; //we are using val field of u1, to pass 
               //the initial value of a semaphore, as 
               //mentioned, in p2
   //semctl() will initialize the first semaphore(index 0/p2 is 0)
   //based on the value of val, in the union - in this case, 
   //it will be set to 1
   //p1 --->id of a semaphore-objecti
   //p2 --->semaphore-index of a semaphore-instance, 
   //       in a semaphore-objecti -->0,1,2,...,
   //       for a single-semaphore instance, index is just 0
   //p3 ---->SETVAL is the command to tell semctl() to 
   //        initialize value of a semaphore-instance, in a
   //        semaphore-objecti, as per p2 and p4
   //
   //p4 is an union used to pass a value to initialize the
   //semaphore-index, as p2 - union's val field is being 
   //using  
   //
   //
   ret1 = semctl(sem_id,0,SETVAL,u1);//setting the semaphore values

   if(ret1<0) { perror("error in semctl setting"); exit(3); }
   //
   //if we change the p3's command value to 
   //GETVAL, semctl() will not set a semaphore's 
   //value, but get a semaphore's value 
   //ret1 contains the return value.....
   //
   //p1--->pass id of semaphore-objecti
   //p2--->pass semaphore-indexi of a semaphore-instancei, in 
   //      a semaphore-objecti
   //p3--->GETVAL command will be used to read the current-value
   //      of semaphore-instancei, as p2 
   //ret1 --->value read will be returned and stored, in 
   //         ret1  
   //
   ret1 = semctl(sem_id,0,GETVAL);//setting the semaphore values

   if(ret1<0) { perror("error in semctl getting"); exit(4); }

   printf("current semaphore value is %d\n", ret1); 

  //we are accessing a shared virtual segment/
  //shared-memory segment, in the 
  //parent process - we are accessing a shared-data 
  //, in a shared virtual segmenti of this processi/pdi
  //what happens, when we access a virtual addressi, 
  //in a shared memory data, in a shared virtual segmenti?i
  //what happens, in the background ??
  //
  //we just use pointers ?? no system call APIs 
  printf("pointer address of shmA is %lx\n", shma);
  printf("pointer address of shma->count is %lx\n", &shma->count);
  printf("pointer address of shma->count1 is %lx\n", &shma->count1);
  printf("pointer address of shma->count2 is %lx\n", &shma->count2);

  shma->count =  3353; //this is the initial value  
  shma->count1 = 4353; //this is the initial value  
  shma->count2 = 5353; //this is the initial value  
  shma->count3 = 6353; //this is the initial value  
  shma->count4 = 7353; //this is the initial value 

 
//pause();
//while(1);
//sleep(10);

  while(i++<1){ //change this with a reasonable condition !!!

   ret = fork();
   //after fork(), codei/datai/heapi/stacki segments 
   //are duplicated,
   //using duplicate-VADs, which are private-VADs -
   //child-process is given duplicate VADs of parent-processi
   //---> 
   //private-VADs mean,  page-frames are private for 
   //these duplicated, private-VADs  
   //however, shared-memory segment's VADs are duplicated, but
   //set/marked, as shared-VADs - what does this mean ??
   //when shared-memory segment's duplicated, VADs are used, 
   //in child-process, they are forced to point to the same,
   //shared-memory objecti and also, their ptes/page-frames
   //are shared, with the parent's ptes/page-frames  
   //
   //
   // 
   //
   if(ret<0){ 
           perror("error in fork"); 
           printf("the final value of i is %lu\n", i);
          
           //an example for normal termination, but not successful 
           exit(1); 
   }

   //parent context 
   if(ret>0){ 
	   printf("I am in parent process context\n"); 
           printf("in parent .. ppid is %lu ...and pid is %lu\n", 
		   getppid(),getpid());	   
         //struct sembuf object's elements 
         sembuf1.sem_num = 0; //semaphore-index value -
                              //we are operating on first-semaphore/index 0
         sembuf1.sem_op = -1; //we are doing a decrement-operation on 
                              //semaphore-instance-0 
         sembuf1.sem_flg = 0; //as always, flags are set to 0 - 
                              //initially, let us ignore flags
         //now, semop() will operate on the first semaphore
         //of our semaphore objecti, with sem_id
         //p1 --->id of a semaphore objecti
         //p2 --->pointer to a structure containing a semaphore 
         //       operation on a semaphore indexi - the table 
         //       contains a single element/object
         //p3 --->is the number of elements/operations, in p2 
         //       - in this case, there is only one operation.... 
	 ret1= semop(sem_id,&sembuf1, 1);//decrementing a semaphore 
         if( ret1<0 ) { perror( "error in semop" ); exit(1); }



  //ret1 = semctl(sem_id,0,GETVAL);//setting the semaphore values

  //if(ret1<0) { perror("error in semctl getting"); exit(4); }

//  printf("semaphore value is %d\n", ret1); 
           while(j++ < 5000000) {   //--->CS11's code-block starts
          // while(j++ < 5000) {
              //start of a critical section code block
              //shared-data access  
                        shma->count++;     //ok
                        shma->count1++;
                        shma->count2++;
              //end of a critical section code block 
           }  //--->CS11's code-blocks ends
           sembuf1.sem_num = 0; //similar to above - semaphore index 0
           sembuf1.sem_op = +1; //increment operation on first semahore
           sembuf1.sem_flg = 0; //flags is 0
   //in this context, semop() will do an increment operation
   //on first semaphore of semobjecti, with sem_id
   //the semop() here is used to do increment operation, 
   //otherwise all the other points mentioned above are the same 
	   ret1= semop(sem_id,&sembuf1, 1);//incrementing a semaphore
           if(ret1<0) { perror("error in semop"); exit(5); }


  //ret1 = semctl(sem_id,0,GETVAL);//setting the semaphore values
  //if(ret1<0) { perror("error in semctl getting"); exit(4); }
 // printf("semaphore value is %d\n", ret1); 
           //read the comments below - if you understand the comments,
           //what happens, if we add waitpid() in this block of code,
           //not in the while loop below 
           //if we use waitpid() here, parent process will be blocked
           //until the recently created child process is terminated and
           //cleaned-up - which means, several children processes cannot 
           //co-exit actively - which means, multiprogramming/multitasking
           //is lost - in addition, try to visualize the same problem
           //in a multiprocessing (MP) environment - the efficiency 
           //will be simply unacceptable !!!

           //
           //using waitpid() is not efficient !!!
           //ret=waitpid() 
printf("finished in parent\n"); 

           continue;
   }
   //it is  a fork() return value/ret
   if(ret==0) { 
	   printf("I am in child process context\n"); 
           printf("in child .. ppid is %lu ...and pid is %lu\n", 
		   getppid(),getpid());	   
//pause();	   
          //this is a simple shared-data access
         sembuf1.sem_num = 0;
         sembuf1.sem_op = -1;
         sembuf1.sem_flg = 0;
	 ret1= semop(sem_id,&sembuf1, 1);//decrementing a semaphore 
         if(ret1<0) { perror("error in semop"); exit(4); }
//  ret1 = semctl(sem_id,0,GETVAL);//setting the semaphore values

//  if(ret1<0) { perror("error in semctl getting"); exit(4); }

//  printf("semaphore value is %d\n", ret1); 
         while(j++<5000000) { //-->CS12's code-block starts
         //while(j++<5000) {
                shma->count--;
                shma->count1--;
                shma->count2--;

         }  //--->CS12's code-block ends
	 sembuf1.sem_num = 0;
         sembuf1.sem_op = +1;
         sembuf1.sem_flg = 0;
	 ret1= semop(sem_id,&sembuf1, 1);//incrementing a semaphore   
         if(ret1<0) { perror("error in semop"); exit(5); }
//   ret1 = semctl(sem_id,0,GETVAL);//setting the semaphore values
//  if(ret1<0) { perror("error in semctl getting"); exit(4); }
//  printf("semaphore value is %d\n", ret1);          
           exit(0); 
   }

 }//while

 //this block of code will be executed only by the parent and 
 //parent will reach here only if it has broken the first while loop 
 //and completed its basic work !!!

 //for most cases of process coding, you must use waitpid() as 
 //shown below - in addition, if you observe, this loop is
 //outside the parent's main execution block - meaning, any such
 //clean up activity must be done by the parent after its 
 //actual work - in addition, if you do not code using this 
 //approach, concurrency and multitasking may be lost !!!

 //the code below is a template for the conditions and loop
 //however, modify it as per your understanding and needs !!!
 //the code below is passive - meaning, just prints information 
 //when you code,you may have to take more actions, actively !!! 
 //fork() return / ret 
//pause(); 

 if(ret>0)
 {
   while(1){ //this while(1) is ok - it has been used with a clear purpose
             //it will break when a certain condition is true - see below !!! 

    //as per man page, first parameter can be >0 
    //however, it cannot be an arbitrary no. - it must be +ve 
    //and a valid pid of a child process of this parent process !!!

    //as per man page, first parameter can also be <0 (-ve)
    //absolute value of the -ve value must be a valid 
    //process group id of a set of  children processes of this 
    //parent process !!!

    //as per man page, first parameter can also be 0 - 
    //in which case, this parent process will wait/cleanup
    //any child process belonging to the group id as that of 
    //parent process !! 

    ret = waitpid(-1,&status,0);
    if(ret>0){

    if(WIFEXITED(status))     //normal termination of the process
    {
       if(WEXITSTATUS(status) == 0){ //normal and successfull
       }
       else{//normal, but not successfull
           //you may have to set a flag to be used later 
       }
    }
    else{ //abnormal (did not succeed)
        //you may have to set a flag here to be used later
    }
   }//ret>0
    //we may use break instead of exit(0) 
    if(ret<0) { 
           //pause();

     printf("final value of shared counter is %d\n", shma->count); 
     printf("final value of shared counter1 is %d\n", shma->count1); 
     printf("final value of shared counter2 is %d\n", shma->count2); 
           shmctl(id,IPC_RMID,0); //destroy the shared memory object !!!
    //printf("final value of shared counter is %d\n", shma->count); 
           exit(0); 

    } //no child is in any state for this process
                           //all the children have terminated and 
                           //cleaned-up by the parent process
   } //second while 
  }//if after while loop 

return 0;
   
}


