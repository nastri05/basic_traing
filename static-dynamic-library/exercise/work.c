#include<stdio.h>
#include<time.h>
#include<test.h>
void delay(int number_of_seconds)
{
	int milli_seconds = 1000 * number_of_seconds;
	clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

void  main(){
	while(1){
		delay(2000);
		increase_value();
		printf("gia tri cua bien A = %d \n",get_value());
	}
}

