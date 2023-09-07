// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define API_HANDLE_DISCONNECT_WAN "disconnect_wan"
#define KEY_HANDLE_DISCONNECT_WAN 65
#define mType_HANDLE_DISCONNECT_WAN 2

// structure for message queue
struct mesg_buffer {
	long mesg_type;
	char mesg_text[100];
} message;

int main()
{
	key_t key;
	int msgid;
    int msgflg;
	// ftok to generate unique key
	key = ftok(API_HANDLE_DISCONNECT_WAN, KEY_HANDLE_DISCONNECT_WAN);

	// msgget creates a message queue
	// and returns identifier
	msgid = msgget(key, 0666 | IPC_CREAT);

	// msgrcv to receive message
	msgrcv(msgid, &message, sizeof(message), mType_HANDLE_DISCONNECT_WAN, 0);
    // printf("Data Received is : %d \n",
	// 				msgflg);
	// display the message
	printf("Data Received is : %s \n",
					message.mesg_text);

	// to destroy the message queue
	msgctl(msgid, IPC_RMID, NULL);

	return 0;
}
