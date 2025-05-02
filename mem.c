#include <stdio.h>
#include <stdlib.h>
const int global;
int global_in = 90;

void foo();

int main(int argc, char *argv[]){

	int a =10;
	int b;

	static int c;
	static int d=10;
        int *dyn = (int*)malloc(sizeof(int));
        	
        
        printf("Address of a:   %p\n",&a);
        printf("Address of b:   %p\n",&b);
        printf("Address of c:   %p\n",&c);
        printf("Address of d:   %p\n",&d);
        printf("Value  of global:   %d\n",global);
        printf("Address of global_in:   %p\n",&global_in);
        printf("Address of dynamic mem:   %p\n",dyn);
	printf("Address of printf: %p\n",printf);
	printf("Address of foo: %p\n", foo);
        printf("Address of argv: %p\n", argv[1]);
	foo();
return 0;
}

void foo(){

	int foo_a;
	
        printf("Address of foo_a:   %p\n",&foo_a);
         while(1);


}


