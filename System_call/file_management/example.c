#include<fcntl.h>
#include<stdio.h>
int main(){
	int fd ;
	char *tmpname ="TMPFILE.txt";
	fd = open(tmpname,O_CREAT | O_RDWR,S_IRWXU );
	if(fd == -1){
		printf("Erorr open file \n");
	}
	write(fd,"hello 2343242",strlen("ello 2343242"));
	//while(1){}
	close(fd);
}
	
