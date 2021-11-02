#pragma once
#include "stdio.h"
class Counter
{
public:
	Counter();
	~Counter();
	int share_count;
	int weak_count;
private:

};

Counter::Counter():share_count(0),weak_count(0)
{
	printf("create Counter\n");
}

Counter::~Counter()
{
	printf("delete Counter\n");
}