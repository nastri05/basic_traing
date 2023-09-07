#include<stdio.h>
#include"add.h"

void __attribute__ ((constructor)) constructorFoo();
void __attribute__ ((destructor)) destructorFoo();

void constructorFoo(){
	printf("Tuan is loaded \n");
}
void destructorFoo(){
	printf("Tuan is closed \n");
}

void PrintfVersion(){
	printf(" Version lib is : %s \n",Version_Lib);
}

int Tuan(){
	return 1;
}
