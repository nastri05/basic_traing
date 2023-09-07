#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
// do not include foo.h, just know the declaration
int main() {
void *ptr;
// declare function pointers
// according to the target function calls
void (*fptr_set)(int); // void setFoo(int f);
int (*fptr_get)(); // int getFoo();
// Open the target dynamic lib
void* foolib = dlopen("libfoo.so", RTLD_LAZY | RTLD_GLOBAL);
if(!foolib) {
printf("ERROR! Can not open libfoo.so\n");
exit(1);
}
// Get function pointers
ptr = dlsym(foolib, "setFoo");
int *iptr = (int*) dlsym(foolib,"setFoo");
printf("Dia chi cua func set Foo : %x \n",iptr);
if(!ptr) {
printf("ERROR! Can not find function setFoo\n");
exit(1);
}
fptr_set = (void (*)(int))ptr;
ptr = dlsym(foolib, "getFoo");

if(!ptr) {
printf("ERROR! Can not find function getFoo\n");
exit(1);
}
fptr_get = (int (*)())ptr;
// Call function via function pointers
printf("Init foo = %d\n", fptr_get());
fptr_set(5);
printf("New foo = %d\n", fptr_get());
while(1);
return 0;
}
