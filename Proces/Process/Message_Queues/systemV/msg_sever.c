#include <sys/msg.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct msgbuf {
   long mType;
   char mText[50];
};

int main() {
   char txtBuf[50];
   int qId;
   key_t key;
   struct msgbuf msg, buf;
   struct msqid_ds msgCtlBuf;
   char str[25];

   if ( ( key = ftok( "msgq.txt", 'C' ) ) == -1 ) {
      perror( "server: ftok failed:" );
      exit( 1 );
   }

   printf( "server: System V IPC key = %u\n", key );

   if ( ( qId = msgget( key, IPC_CREAT | 0666 ) ) == -1 ) {
      perror( "server: Failed to create message queue:" );
      exit( 2 );
   }

   printf( "server: Message queue id = %u\n", qId );

   strcpy( msg.mText, "This is a message" );
   msg.mType = 1;

   if ( msgsnd( qId, &msg, sizeof msg.mText, 0 ) == -1 ) {
      perror( "server: msgsnd failed:" );
      exit( 3 );
   }

   printf( "server: Message sent successfully\n" );

   printf( "server: waiting..." );
   //sleep( 15 );
   int  i;
   for(i = 0;i<5;i++){
   printf("Enter number : ");
   scanf( " %24[^\n]", str);
   strcpy( msg.mText,str);
   if ( msgsnd( qId, &msg, sizeof msg.mText, 0 ) == -1 ) {
      perror( "server: msgsnd failed:" );
      exit( 3 );
   }
   printf( "server: done waiting. removing message queue...\n" );
   }

   printf( "server: Message queue removed OK\n" );
}