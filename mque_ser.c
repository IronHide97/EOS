#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#define KEY1 1552

struct message {
    long mtype;
    char mtext[100];
};

int main() {
    int msgqid, ret1;
    struct message msg;


    // Create or get the message queue
    msgqid = msgget(KEY1, 0666 | IPC_CREAT);
    if (msgqid <0 ) {
        perror("msgget");
        exit(1);
    }

    // Prepare the message to send
    msg.mtype = 2;
    strcpy(msg.mtext, "Hello, message queue!");

    // Send the message
    ret1 = msgsnd(msgqid, &msg, sizeof(struct message), 0); 
    if (ret1 < 0)
    {
        perror("msgsnd");
        exit(1);
    }

    printf("Message sent successfully.\n");

    return 0;
}
