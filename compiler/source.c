#include"header.h"

#ifndef SPEED_INIT
#define SPEED_INIT 0
#endif

int spd = SPEED_INIT;
void main(){
while(1){
spd = SPEED_UP(spd);
}
}
