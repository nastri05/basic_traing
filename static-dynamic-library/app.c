#include<foo.h>
#include<stdio.h>
int main(){
	printf("Init foo = %d\n", getFoo());
	setFoo(5);
	printf("New foo = %d\n", getFoo());
	while(1);
	return 0;
//
}
