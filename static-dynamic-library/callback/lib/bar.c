#include<stdio.h>
extern void callback();
void run(){
	printf("Run from BAR!\n");
	callback();
}
