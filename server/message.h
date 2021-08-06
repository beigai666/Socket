#pragma once
struct Message
{
	int ack;
	int leng;
	char buf[0];
};

Message* create_message(const char* buf, int leng);

int disassemble(Message* msg);

Message* getMsg(Message* msg);

void deleteMessage(Message*& msg);