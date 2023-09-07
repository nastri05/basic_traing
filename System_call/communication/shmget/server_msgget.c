// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX 10
#define API_HANDLE_DISCONNECT_WAN "server_send_handle_disconnect"
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

	// ftok to generate unique key
	key = ftok(API_HANDLE_DISCONNECT_WAN,KEY_HANDLE_DISCONNECT_WAN);

	// msgget creates a message queue
	// and returns identifier
	printf("value %d \n",key);
	msgid = msgget(key, 0666 | IPC_CREAT);
    if(msgid ==-1 ){
        printf("Send message fail \n");
        return -1;
    }
	message.mesg_type = mType_HANDLE_DISCONNECT_WAN;

	printf("Write Data : ");
	fgets(message.mesg_text,MAX,stdin);

	// msgsnd to send message
	msgsnd(msgid, &message, sizeof(message), 0);

	// display the message
	printf("Data send is : %s \n", message.mesg_text);

	return 0;
}
