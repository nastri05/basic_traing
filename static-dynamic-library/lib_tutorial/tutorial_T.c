#include<stdio.h>
#include"tutorial_T.h"
void __attribute__ ((constructor)) constructorLib();
void __attribute__ ((destructor )) destructorLib();

void constructorLib(){
	printf("Lib math is Loaded \n");
}
void destructorLib(){
	printf("Lib math is Closed \n");
}

int Tuan(int a, int b){
	return (a >b)? 1:0;
}
int Dante(int a, int b){
	return (a < b)? 1:0;
}

