#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main(){
	int fd ;
	char *parent_chr = "Hello Parrent \n";
	char *child_chr = "Hello Child \n";
	char *tmpname ="TMPFILE.txt";
	
	if(fd == -1){
		printf("Erorr open file \n");
	}
	if(fork()==0){
		fd = open(tmpname,O_CREAT | O_RDWR,S_IRWXU );
		write(fd,child_chr,sizeof(child_chr));
		printf(" Value of fd = %d \n",fd);
	}else{
		fd = open(tmpname,O_CREAT | O_RDWR,S_IRWXU );
		write(fd,parent_chr,sizeof(parent_chr));
		printf(" Value of fd = %d \n",fd);
	}
	close(fd);
	
	return 0;
}