#include<stdbool.h>
#include"header.h"

#ifndef SPEED_INIT
#define SPEED_INIT 0
#endif

int spd = SPEED_INIT;
void main(){
while(true){
spd = SPEED_UP(spd);
}
}
