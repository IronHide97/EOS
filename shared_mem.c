#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include <sys/sem.h>
#include<sys/shm.h>


//we need to use unique KEY values, for creating
//a shared-memory IPC mechanism instance

#define KEY1 1555

//explicit shared data, which will be part 
//of a shared-data segment of processes -
//this shared-data object will be placed, in a
//shared-data segment
//we will be using pointer techniques to place
//this object, in a shared memory segment
//
// 
struct shmarea {   //this is a shared-data object, 
                   //in a shared-segment, in user-space

  unsigned long count;     //a shared-data variable
  unsigned long count1;    //another, shared-data variable
  unsigned long count2;    //""""
  unsigned long count3;    //""""
  unsigned long count4;    //""""""""

}; 

//this pointer variable is related to our shared-memory 
//object above 
//
struct shmarea *shma; //we will store a shared-data segment's
                      //starting virtual address, in this 
                      //variable ??

int main()
{
  //
  //be warned - if we re-use return variables, in 
  //            certain places, program logic will break
  //
  //check you code for return variable re-usage and
  //program logic
  //
   int ret,ret1,ret2,status,id;
  
   unsigned long int i=0, j=0;//these iteration variables
                              //used

     //p1 is used to pass an unique KEY value, for each 
     //shared-memory object - in this case, 
     //we are using a hard-coded KEY value - if you want to 
     //know more rules, refer to man shmget
     //
     //p2 will be the shared-memory IPC's shared memory 
     //segment size(it is the same as shared physical memory size
     // - as per rules, must be a multiple of 
     //page-size ---> 4096(a single page)/8192(2 pages)/(multiple pages)....  
     //
     
     id =  shmget(KEY1,2*2*getpagesize(),IPC_CREAT|0600);
     if(id <0) { perror("error in shmget"); exit(5); }
      
     //for now, let us use a standard set of flags, for 
     //p3 .....
     //in the above shmget(), p3 is used to set the 
     //multi-user access permissions, for this shared-memory 
     //object/segment - 0600 is the minimum permissions, for
     //the current user/owner of this shared-memory 
     //object - owner is the user-id -- meaning, all processes 
     //of this owner/user-id are allowed to access this 
     //shared-memory object and its segment(s) - if needed, 
     //we can change it to 0660 or 0666, for granting 
     //permissions to other user-ids /processes
     //if we need to create a new shared-memory object, 
     //we must use IPC_CREAT - IPC_CREAT will create a 
     //new shared-memory object, if already not created-
     //otherwise, will just return the handle/id of the
     //existing shared-memory object for the given KEY value
     // 
     //refer to man shmget(), for more details 
     //id provides handle to a shared-memory object 
     //associated, with this KEY value 

 
   //param2 - 0 means, system allocates virtual addresses for
   //shared virtual pages/VADs !!! you may use non-zero values, but
   //not preferred !!
   //param3 - flags are initially set to 0 !!! ignore the flags, 
   //for time-being....
   //we may use flags to set certain specific access permissions !!!
   shma = shmat(id,0,0); 

	//---->will the mappings of shared page-frames be 
	//     set-up, after shmat() completes - meaning,
	//     will the ptes of shared virtual pages 
	//     be mapped to corresponding shared page-frames
	//     of shared-memory objecti ?? refer to a long
	//     story described, in ipcs_2.txt - this is 
	//    based on demand-paging .....


	//--->if shmat() is successful, we are effectively placing 
	//    our shared-memory data type, in a shared-memory 
	//    virtual segment....
	//next, we will access our shared-memory object, using
	//this pointer and certain pointer arithmetic .... 


  //we are accessing a shared virtual segment/
  //shared-memory virtual page, in the 
  //parent process - we are accessing a shared-data 
  //, in a shared virtual page of this processi/pdi-
  //what happens, when we access a virtual addressi, 
  //in a shared memory data, in a shared virtual segmenti?
  //what happens, in the background ?? a form of 
  //demand paging, but for shared page-frames -
  //we just use pointers ?? no system call APIs
   
  shma->count = 5533; //this is the initial value  
  shma->count1 = 6533; //this is the initial value  
  shma->count2 = 7533; //this is the initial value  
  shma->count3 = 8533; //this is the initial value  
  shma->count4 = 9533; //this is the initial value  

  while(i++<1){ //change this with a reasonable condition !!!
//     for a long story on 
//     fork() and shared-memory segments/object set-up
//
   ret = fork();
   //after fork(), codei/datai/heapi/stacki segments 
   //are duplicated
   //using duplicate VADs, which are private VADs - 
   //private VADs mean,  page-frames are private for 
   //these private VADs  
   //however, "shared memory data-segment" VADS/address-descriptors
   // are duplicated, but
   //set/marked, as shared VADs - what does this mean ??
   //when shared segment VADs are used and point to the same
   //shared memory objecti, their respective shared page-frames
   //are shared and their ptes are set to point to the 
   //same set of shared page frames !!!
   //for a longer story, refer to 11_ipcs_2.txt 
   //
   //-->after fork(), private-segments will be treated as private-segments
   //-->after fork(), shared-segments will be treated as shared-segments
   // 
   //
   if(ret<0){ //fork() failed 
              // 
           perror("error in fork"); 
           printf("the final value of i is %lu\n", i);
          
           //an example for normal termination, but not successful 
           exit(1); 
   }

   //parent context - a block of code  
   if(ret>0){ 
	   printf("I am in parent process context\n"); 
           printf("in parent .. ppid is %lu ...and pid is %lu\n", 
		   getppid(),getpid());	   
        
           while(j++ < 6000000) {
              //start of a critical section code block
              //shared-data access  
                        shma->count++;     //ok
                        shma->count1++;
                        shma->count2++;
              //end of a critical section code block 
           }
           

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
   if(ret==0) { //child instance 
	   printf("I am in child process context\n"); 
           printf("in child .. ppid is %lu ...and pid is %lu\n", 
		   getppid(),getpid());	   
         //pause();	   

         while(j++<6000000) {
                shma->count--;
                shma->count1--;
                shma->count2--;

         }        
           exit(0); 
   }

 }
 //while

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
 if(ret>0)
 {
   while(1){ //this while(1) is ok - it has been used with a clear purpose
             //it will break when a certain condition is true - see below !!! 
//pause();

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
    //--->shmctl(id,IPC_RMID,0); //destroy the shared memory object !!!
    //printf("final value of shared counter is %d\n", shma->count); 
           exit(0); 

    } //no child is in any state for this process
                           //all the children have terminated and 
                           //cleaned-up by the parent process
   } //second while 
  }//if after while loop 

return 0;
   
}


