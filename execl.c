#include<stdio.h>
       #include <sys/types.h>
       #include <unistd.h>

int main(){
 
 int ret;

  int age = 10; 
  printf("Hi Master\n");
   ret = fork();
  if (ret == 0 ){
     printf("I'm in Child process pid: %d, ppid:%d\n", getpid(),getppid());
     printf("Age:%d\n",age);
     execl("/home/shubham/DESD_EOD/Session2/temp", "temp", NULL);
  }
  
 else  if (ret > 0){
     printf("I'm in Parent procss pid: %d, ppid;%d\n",getpid(),getppid());
     printf("Age:%d\n",age);
     while(1);
   }
 
 printf("Failed to create child");
return 0;
}
