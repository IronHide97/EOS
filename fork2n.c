#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

int data1=0; //a global data in read-write segment  

int main()
{

   int ret,status;

   unsigned long int i=0;
  //block1
  while(i++<5){ //change this with a reasonable condition !!!
   //we are creating 5 children processes and managing 
   //these , from a common parent process 
   ret = fork();  //ret > 0 -->in parent-context, if successful 
                  //ret < 0 -->in parent-context, if unsuccessful
                  //ret == 0 -->in child-context, if successful 

   //if fork() system-call API fails, due to certain 
   //resource/restrictions, in the OS- it will return -1, 
   //in the parent-process' context - this must be 
   //treated, as a serious-error - the parent process
   //code will terminate, using exit(n) - n !=0 ;  
   //the application is said to be unsuccessful,as 
   //multitasking set-up failed 
   //---->system may not have resources or certain 
   //     restrictions - due to this, a new, child- 
   //    process is not created...
   //
   //part of block1 - conditionally executed, in parent-processi
   if(ret<0){ 
           //typically, perror() can print an error 
           //string , based on the error code of 
           //the previous system call API 
           perror("error in fork"); 
           printf("the final value of i is %lu\n", i);
         
           //if the parent fails to create a new process, 
           //it will be treated as an error of the 
           //applicationi and exit(1) is invoked 
           //since it is the first error, exit code is 
           //1 - exit() is the termination system call 
           //API, which will normally terminate the processi
           //and store the exit code passed, in the pdi 
           //of the zombie process(parent is the current processi)

           ///this exit status code, along with termination 
           //status code will be present, in the pd  
 
           exit(1);//we have chosen 1, as the exit-code
                   //- meaning, there is a failure, so 
                   // application is unsuccessful - 
                   //this status-code will be stored, as 
                   //part of termination-status information
                   //of the pdi of this zombie, parent-process   
   }
   //if a fork() system call API is successful, 
   //the fork() system call will complete and 
   //return  a +ve value to the parent processi/context of
   //execution, 
   //which is the pidi+1 of the newly, created child-processi+1 -
 
   //the following block of code will be processed, in 
   //the parent process context only, not in the child
   //process context - to identify the parent context 
   //of execution, we need to check the ret value of 
   //fork(), in our code - we print the process id and parent 
   //process id of the parent process 
   //(parent process of the current process)  
   //part of block1 of the current, program-image 
   if(ret>0){ //conditionally executed, in the parent 
              //context,  only   
	   printf("I am in parent process context\n"); 
           printf("in parent .. ppid is %lu ...and pid is %lu\n", 
		   getppid(),getpid());	   
           printf("in parent .. ppid is %lu ...and main's address is %lx\n", 
		   getppid(),&main);	   
           printf("in parent .. ppid is %lu ...and  address of data1 is %lx\n", 
		   getppid(),&data1);	   
	   //
           //
           //job1 , in the parent's context of execution
           //       instance 
           // 
           //++i;     //ok
           //break;

   //in the parent process context, let us continue the job, 
   //as part of the first block of code,which is to 
   //create multiple children processes  
   //in this code sample, the job of the parent process is 
   //to create multiple children processes only  
           continue;
   }
   //this block of code executes, in the context of 
   //a child process only and does appropriate job assigned 
   //to this block of code,  in the 
   //context of the child process only - we need to check 
   //the context of child, using return value of 
   //fork() - if it is 0, we are, in a child-process,
   //context of execution  
   //if we have multiple children processes, we can 
   //have multiple jobs, in different children-processes - 
   //a form of multitasking  
   //we also print pid and ppid of the children processes  
   //code-block2 of this program-image
   if(ret==0) { 
	   printf("I am in child process context\n"); 
           printf("in child .. ppid is %lu ...and pid is %lu\n", 
		   getppid(),getpid());	   
           printf("in child .. ppid is %lu ...and main's address is %lu\n", 
		   getppid(),&main);	   
           printf("in child .. ppid is %lu ...and  address of data1 is %lu\n", 
		   getppid(),&data1);	   
           //for each child process, we can assign 
           //a different job 	   

           //for i==1, the first instance of the 
           //child process is doing the job 
           //similarly, for other i values, 
           //other instances of the children processes
           //will do their jobs 

           //job of the child - do any work in the child process
	  if(i==1 || i==2) exit(0);
           if(i==3) exit(1);  
           if(i==4) exit(2);  
           while(1); //we have added a dummy job to each 
                     //child process          
          //exit(0); 
          //  break;
           //continue;
   }

 }//while  ---->we need to visualize and connect the details...


   //pause(); //used, for testing - this will block the parent processi 
 

//this is another block of code, 
 //which  will be executed only, by the parent 
 //processi and 
 //parent processi will reach here , if it has completed the 
 //the previous code-blocks of this program assigned to the parent processi 
 // 
 //previous/first code-block of parent process does basic work of setting up 
 //multiple children-processes, in this program !!!
 //
 //
 //this block of code will do clean-up of zombie,
 // children-processes of this parent-process
 if(ret>0)//this is to check, the current-process is 
           //the parent-process 
 {
   //pause(); //used, for testing - this will block the parent processi 
   while(1){ 

//this while(1) is ok - it has been used with a 
//clear purpose
//this loop will break, when a certain condition is true - 
//see below !!! 

//as part of process-management design of Unix/Linux, 
//waitpid() is typically used, in the parent-processi's
//context of execution, 
//to clean-up terminated/zombie, children-processes  
//
//waitpid() does not terminate children-processes, 
//but cleans-up 
//terminated, zombie,  children-processes - if a child-processi+1 is 
//not, in terminated/zombie-state, waitpid() will not 
//do anything to this child-processi - if there is a terminated
//child processi+1, waitpid()  will delete 
//pdi+1 of the child processi+1, 
//after collecting termination, status-code of pdi+1 - termination
//status of pdi+1 will be copied into param2 of waitpid() -   
//first parameter of waitpid() can be set to -1, if we wish to use 
//waitpid(),
//to clean-up any, terminated, child-processi+1 of this
//parent-processi,not a 
//specific, child-processi+1 of this processi - we will be using
//this value, for param1
//
//===========================================================
//first parameter can be a +ve value(pidi+1 of a specific 
//child processi+1, if we wish to clean-up
//a specific, terminated(zombie), child-processi+1) - we will
//not be using this value, for param1
//===========================================================
// 
//in our cases, we will be using -1, for cleaning-up
//any child-processi+1 - 
//using -1 is a common case - if we pass -1, it means, 
//that waitpid() will clean-up any, terminated, child-
//processi+1 of this parent processi 
//
//second-parameter is used to collect the termination, 
//status-code of a zombie child-processi+1,during 
//clean-up  - when waitpid()
//cleans-up a terminated, child-processi+1,it will collect 
//the termination, status-code from the pdi+1 and fill 
//the second-parameter, status field - in this context, p2 is an 
//address/pointer of an user-space variable to a system-call API - this 
//pointer will be used, by the system-call service- 
//routine, to copy termination, status-code of pdi+1 into
//an user-space variable...
//
//   
//last parameter is flags, which we do not use normally, so
//set to 0 - we are not using flags, so we set it to 0 -
//if we set this flag to 0, waitpid() will behave normally -
//this is, what we want - this is also the regular-usage, so
//flags are set to 0 
// 
//
//based on the current set of parameters, let us understand the 
//behaviour of waitpid(), in this parent process, when
//it is invoked ??
//
//
//--->in this parent-processi's current execution, 
//      following is a scenario-1: 
//
//if waitpid() is invoked, in this parent-processi 
//and a terminated/zombie 
//child processi+1 
//is waiting for clean-up, waitpid() system-call will collect the 
//the termination, status-code of pdi+1 and store it, in p2's location, and 
//complete the 
//clean-up of the zombie child-processi+1/pdi+1 - delete pdi+1 of the 
//terminated, child-processi+1 - foot-print of a terminated
//child-processi+1 is deleted, in the system - once clean-up processing 
//is complete, waitpid() will return  
//the pidi+1 of 
//the just cleaned-up processi+1/pdi+1 - this will be the return value 
//of ret = waitpid(p1,p2,p3);
//
//
//--->in this parent-processi, following is another scenario-2: 
//
//if waitpid(-1, &status, 0) is invoked and 
//currently, there are no 
//terminated/zombie, children-processes of this 
//parent-processi , waitpid() will block
//the parent-processi, until any, one child-processi+1 of
//this parent-processi is 
//terminated and enters zombie-state - 
//waitpid(-1, &status, 0) is a blocking, system-call API,for
//certain conditions - in the near future, 
//whenever a child-processi+1 is 
//terminated, it will enter the zombie-state - 
//since the waiting-condition is satisfied, 
//the parent-processi+1 is  unblocked, in the 
//background  - added to ready-state/ready-queue - 
//in the near future, 
//this parent-process will be scheduled/
//dispatched/executed - the above unblocking and scheduling/
//dispatching is done, in the background   
//- after the unblocking and 
//scheduling/dispatching of the parent-processi, 
// waitpid() will be resumed and 
// it will complete its job - clean-up of a terminated child-process 
//will completed and waitpid() will return a +ve value
//(pid of zombie pdi+1) - in addition, termination, status-code
//will be copied into param2's location....
//waitpid() is a blocking system-call, in this context -  
//
    ret = waitpid(-1,&status,0); //better, for this scenario
    //ret = waitpid(pid_ary[i],&status,0); //not appropriate
                                           //for this 
                                           //scenario
            
    if(ret>0){ //ret will contain pidi+1 of a child-processi+1
//if waitpid() successfully cleans-up a terminated,
//child processi+1, 
//it will collect and copy the status-code,into 
//param2's location and also return
//a +ve value of the pidi+1 of the 
//cleaned-up child processi+1, in ret  
//
//we cannot use termination, status-code stored, in
//"status" user-variable containing  
//termination, status-code and exit status code,directly - 
//do not print the values - they do not make any, sense
//
// 
//
//using appropriate OS-macros, interpret the status value  --  
//if  WIFEXITED(status) returns TRUE, the child-processi+1
//terminated normally and exit(0) or exit(n) was invoked, 
//in the code of child processi+1 --
//if the WIFEXITED(status) returns FALSE, the child-processi+1 was
//terminated, abnormally/forcibly and an Unix/Linux signal was involved, 
//implicitly or explicitly -- 
//WIFEXITED(status) uses "termination, status-code stored", 
//in "status" variable, to check, for normal or abnormal, termination
//of the cleaned-up zombie child-processi+1/pdi+1   
//
//
    if(WIFEXITED(status)) //normal-termination of the 
                          //child-processi+1 
    {
//if there is a normal termination, following code block is used
//
//WEXITSTATUS(status) will provide 0, if the childi+1's
//active-applicationi 
//completed, successfully and invoked exit(0) ---
//in this context, applicationi completes successfully and 
//the processi+1 terminates normally
//WEXITSTATUS(status) will extract the  "exit status code"
//stored, in the status variable - if this value is 0, 
//it means, the zombie childi+1 terminated normally, as well
//as the corresponding active applicationi completed 
//successfully - completed its job
//
//however, if the WEXITSTATUS(status) return non-zero 
//value(the stored exit, status-code, in status),
//the zombie, child-processi+1 terminated, normally, but
//the active-applicationi did not complete its job, 
//successfully - there was some error, in the 
//code execution 
//
       if(WEXITSTATUS(status) == 0){ //normally and successfully, completed
          printf("normal and successful termination of pid %d\n", ret);
       }
//in this case, child's application completed, 
//unsuccessfully and normally 
       else{    //a +ve value ; normal, but not successful 
                //completion of application/job
                
          printf("normal and unsuccessful of pid %d\n", ret);

       }
    }

//in addition, there can be another case, where
//the child processi+1 is forcibly terminated and 
//child's applicationi was unable to complete the job -
//such abnormal or forced termination may be 
//due to bugs , in the application or 
//resource problems, in the system ---
//if WIFEXITED(status) returns FALSE, the child processi+1
//terminated abnormally - there was a forced termination 
//of the child processi+1 - if a child processi+1
// is abnormally terminated/
//forcibly terminated, its active applicationi will also 
//be forced to abort execution - it should be treated 
//as an incomplete execution of the applicationi's  code 
//
// 
    else{ //abnormal (did not succeed)
          printf("abnormal and unsuccessful pid %d\n", ret);
    }
   }//ret>0 //waitpid()'s ret is > 0 
//waitpid() returns -1, when all the children processes of
//this parent processi 
//are cleaned-up and there are no more children 
//processes of this parent processi to clean-up -
//in this context, this block of code has completed
//its job and hence terminates the parent process
    if(ret<0) { 
                 exit(0); 
                 //we exit() or break, from the while(1) loop of waitpid() 
    }            //no child is, in any state for this parent process
                           //all the children have terminated and 
                           //cleaned-up by the parent process
   } //second while 
  }//if 

return 0; //we can replace with exit(0)
   
}


