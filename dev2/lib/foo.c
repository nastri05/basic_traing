#include<stdio.h>
#include"foo.h"

int __ffo;

void __attribute__ ((constructor)) constructorFoo();
void __attribute__ ((destructor)) destructorFoo();

void constructorFoo(){
	printf("Foo is loaded \n");
}
void destructorFoo(){
	printf("Foo is closed \n");
}

void setFoo(int v){
	__ffo = v;
}
int getFoo(){
	return __ffo;
}