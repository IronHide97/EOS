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

    // Get the message queue
    msgqid = msgget(KEY1, 0666 | IPC_CREAT); 
    if (msgqid < 0){
        perror("msgget");
        exit(1);
    }

    // Receive the message
    ret1 = msgrcv(msgqid, &msg, sizeof(struct message),1, 0);
     if (ret1 < 0) {
         perror("msgrcv");
         exit(1);
      }

    printf("Received message: %s\n", msg.mtext);

    return 0;
}
