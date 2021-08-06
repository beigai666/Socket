#include "message.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
Message* create_message(const char *buf,int leng) {
	Message* msg = (Message*)malloc(leng + sizeof(Message));
	memset(msg,0, leng + sizeof(Message));
	memcpy(msg->buf,buf,leng);
	msg->ack = 0xff00ff00;
	msg->leng = leng;
	return msg;
}

int disassemble(Message* msg) {
	int leng = msg->leng;
	char* buf = msg->buf;
	printf("buf = %s,leng = %d\n",buf,leng);
	return 0;
}

Message* getMsg(Message* msg) {
	Message* new_msg = (Message*)malloc(msg->leng + sizeof(Message));
	memset(new_msg, 0, msg->leng + sizeof(Message));
	new_msg->ack = 0xff00ff00;
	new_msg->leng = msg->leng;

	return new_msg;
}

void deleteMessage(Message * &msg) {
	free(msg);
	msg = NULL;
}