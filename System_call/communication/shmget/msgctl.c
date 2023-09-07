#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

// Define a message structure
struct message {
  long mtype; // Message type
  char mtext[80]; // Message text
};

int main() {
  printf(" Process ID : %d \n", getpid());
  int msqid; // Message queue identifier
  key_t key; // Key for the message queue
  struct msqid_ds buf; // Buffer for message queue information
  struct message msg; // Message to be sent or received
  int msgflg; // Message flag for msgget and msgsnd
  int msgsz; // Message size for msgrcv
  long msgtyp; // Message type for msgrcv

  // Generate a key for the message queue
  key = ftok("msgctl.c", 'A');
  if (key == -1) {
    perror("ftok");
    exit(1);
  }

  // Create a message queue with read and write permissions for user, group, and others
  msgflg = IPC_CREAT | 0666;
  msqid = msgget(key, msgflg);
  if (msqid == -1) {
    perror("msgget");
    exit(1);
  }

  // Print the message queue identifier
  printf("Message queue identifier: %d\n", msqid);

  // Get the information about the message queue and print it
  if (msgctl(msqid, IPC_STAT, &buf) == -1) {
    perror("msgctl IPC_STAT");
    exit(1);
  }
  printf("Message queue information:\n");
  printf("Owner UID: %d\n", buf.msg_perm.uid);
  printf("Owner GID: %d\n", buf.msg_perm.gid);
  printf("Permissions: %o\n", buf.msg_perm.mode);
  printf("Number of messages: %ld\n", buf.msg_qnum);
  printf("Maximum bytes: %ld\n", buf.msg_qbytes);
  printf("Last send time: %ld\n", buf.msg_stime);
  printf("Last receive time: %ld\n", buf.msg_rtime);
  printf("Last change time: %ld\n", buf.msg_ctime);
  printf("Last send PID: %d\n", buf.msg_lspid);
  printf("Last receive PID: %d\n", buf.msg_lrpid);

  // Send a message of type 1 to the message queue
  msg.mtype = 1;
  sprintf(msg.mtext, "Hello, this is a message of type %ld", msg.mtype);
  msgflg = IPC_NOWAIT;
  if (msgsnd(msqid, &msg, sizeof(msg.mtext), msgflg) == -1) {
    perror("msgsnd");
    exit(1);
  }
  
  // Send another message of type 2 to the message queue
  msg.mtype = 2;
  sprintf(msg.mtext, "Hello, this is a message of type %ld", msg.mtype);
  msgflg = IPC_NOWAIT;
  if (msgsnd(msqid, &msg, sizeof(msg.mtext), msgflg) == -1) {
    perror("msgsnd");
    exit(1);
  }

  // Get the information about the message queue and print it
  if (msgctl(msqid, IPC_STAT, &buf) == -1) {
    perror("msgctl IPC_STAT");
    exit(1);
  }
  printf("Message queue information:\n");
  printf("Owner UID: %d\n", buf.msg_perm.uid);
  printf("Owner GID: %d\n", buf.msg_perm.gid);
  printf("Permissions: %o\n", buf.msg_perm.mode);
  printf("Number of messages: %ld\n", buf.msg_qnum);
  printf("Maximum bytes: %ld\n", buf.msg_qbytes);
  printf("Last send time: %ld\n", buf.msg_stime);
  printf("Last receive time: %ld\n", buf.msg_rtime);
  printf("Last change time: %ld\n", buf.msg_ctime);
  printf("Last send PID: %d\n", buf.msg_lspid);
  printf("Last receive PID: %d\n", buf.msg_lrpid);
  
  // Receive any message from the message queue
  msgsz = sizeof(msg.mtext);
  msgtyp = 0; // Any type
  if (msgrcv(msqid, &msg, msgsz, msgtyp, IPC_NOWAIT) == -1) {
    perror("msgrcv");
    exit(1);
  }
  
   // Print the received message
   printf("Received message: %s\n", msg.mtext);

   // Receive a message of type 2 from the message queue
   msgsz = sizeof(msg.mtext);
   msgtyp =2; // Type 2
   if (msgrcv(msqid, &msg, msgsz, msgtyp, IPC_NOWAIT) == -1) {
     perror("msgrcv");
     exit(1);
   }
   
   // Print the received message
   printf("Received message: %s\n", msg.mtext);

   // Delete the message queue from the system
   if (msgctl(msqid, IPC_RMID, NULL) == -1) {
     perror("msgctl IPC_RMID");
     exit(1);
   }

   return(0); 
}

