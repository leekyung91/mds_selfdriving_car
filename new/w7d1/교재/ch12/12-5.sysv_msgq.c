#include <stdio.h>
#include <stdlib.h>
#include <linux/ipc.h>
#include <linux/msg.h>

/* Redefines the struct msgbuf */
typedef struct mymsgbuf
{
	long mtype;
	int old;
	char name[80];
//	long mtype;	// TODO: try moving mtype here and rerun this program. compare the result. 
} mess_t;

int main()
{
	int qid;
	key_t msgkey;
	mess_t sent;
	mess_t received;
	int length;
	int i;

	/* Initializes the seed of the pseudo-random number generator */
	srand (time (0));
	/* Length of the message */
	length = sizeof(mess_t) - sizeof(long);
	msgkey = ftok(".",'m');

	/* Creates the queue*/
	qid = msgget(msgkey, IPC_CREAT | 0660);
	printf("QID = %d\n", qid);

	/* Builds & sends a message */
	sent.mtype = 1;	sent.old = 13; strcpy(sent.name, "lucy");
	msgsnd(qid, &sent, length, 0); printf("MESSAGE SENT...\n");

	sent.mtype = 2;	sent.old = 13; strcpy(sent.name, "jerry");
	msgsnd(qid, &sent, length, 0); printf("MESSAGE SENT...\n");

	sent.mtype = 3;	sent.old = 13; strcpy(sent.name, "paul");
	msgsnd(qid, &sent, length, 0); printf("MESSAGE SENT...\n");

	sent.mtype = 4;	sent.old = 13; strcpy(sent.name, "bill");
	msgsnd(qid, &sent, length, 0); printf("MESSAGE SENT...\n");

	sent.mtype = 5;	sent.old = 13; strcpy(sent.name, "james");
	msgsnd(qid, &sent, length, 0); printf("MESSAGE SENT...\n");

	/* Receives the message */
	printf("[Receive Type #1]\n");
	i = 0;
	while(1){
		if(msgrcv(qid, &received, length, 5, IPC_NOWAIT) <= 0)
			break;
		i++;
		printf("%d)received.mtype(%d), received.old(%d), %s\n", i, received.mtype, received.old, received.name);
	}

	printf("\n[Receive Type #2]\n");
	i = 0;
	while(1){
		if(msgrcv(qid, &received, length, -2, IPC_NOWAIT) <= 0)
			break;
		i++;
		printf("%d)received.mtype(%d), received.old(%d), %s\n", i, received.mtype, received.old, received.name);
	}

	printf("\n[Receive Type #3]\n");
	i = 0;
	while(1){
		if(msgrcv(qid, &received, length, 0, IPC_NOWAIT) <= 0)
			break;
		i++;
		printf("%d)received.mtype(%d), received.old(%d), %s\n", i, received.mtype, received.old, received.name);
	}

	/* Destroys the queue */
	msgctl(qid, IPC_RMID, 0);
}
