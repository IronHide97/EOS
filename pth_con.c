#include<stdio.h>
#include <pthread.h>


pthread_mutex_t mut1;
pthread_cond_t con_val;

int global;
void si_cl(void *arg){
        char *str =(char *)arg;

	printf("%s\n",str);
	pthread_mutex_lock(&mut1);	
        
        for(int i=0; i<=100000; i++,global++);

	pthread_mutex_unlock(&mut1);
};

void a_cl(void *arg){

        char *str =(char *)arg;
	printf("%s\n",str);
	

	pthread_mutex_lock(&mut1);	

        for(int i=100000; i<=0; i--,global--);
        	
	pthread_mutex_unlock(&mut1);
	pthread_cond_signal(&con_val);	

};



int main(){

char thName[] = "INCREMENT";
char thName2[] = "DECREMENT";
pthread_t th1,th2;

	pthread_mutex_init(&mut1,NULL);
	pthread_create(&th1,NULL,&si_cl,thName);
	pthread_create(&th2,NULL,&a_cl,thName2);
        
	printf("\n BACK in MAIN THREAD\n");\
        
        pthread_mutex_lock(&mut1); 
        pthread_cond_wait(&con_val,&mut1);	
	printf("Final: %d\n",global);
	
	pthread_join(th1,NULL);
	pthread_join(th2,NULL);
	pthread_mutex_destroy(&mut1);
	
return 0;
}
