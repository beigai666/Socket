// socket.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <stdio.h>
#include <iostream>
#include <thread>
#include <map>
#include <string>
#include <Windows.h>
#include "Array.h"
#include "HeapArray.h"
class MyClass
{
	std::thread* th;
public:
	HANDLE m_Event;
	int num;
	MyClass();
	
	void run() {
		while (true)
		{
			std::cout << "SetEvent" << std::endl;
			SetEvent(m_Event);
			Sleep(1000);
		}
	}
	~MyClass();
	void wait() {
		//执行到此处，等待500毫秒直到超时或 SetEvent(m_Event);被调用 
		//如果500毫秒内SetEvent(m_Event);被调用，500毫秒后才返回到此处运行
		int res = WaitForSingleObject(m_Event, INFINITE);
		switch (res)
		{
		case WAIT_OBJECT_0:
			std::cout << "WAIT_OBJECT_0"<<std::endl;
			// m_Event所代表的进程在500毫秒内结束
			break;

		case WAIT_TIMEOUT:
			// 等待时间超过5秒
			std::cout << "WAIT_TIMEOUT" << std::endl;
			break;

		case WAIT_FAILED:
			// 函数调用失败，比如传递了一个无效的句柄
			std::cout << "WAIT_FAILED" << std::endl;
			break;
		}
	}
private:

};

MyClass::MyClass()
{
	num = 0;
	m_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	th = new std::thread(&MyClass::run, this);
}

MyClass::~MyClass()
{
}
int main()
{
	
	
	Array<std::string, 10>arr;
	arr.set(0,"hello");
	arr.set(1, "hhhhhh");
	for (int i = 0; i < 10;i++) {
		std::cout << "arr[" << i << "]=" << arr[i] << std::endl;
	}
	HeapArray<int>* heap = HeapArray<int>::NewInstance(10);
	
	for (int i = 0; i < 10; i++) {
		heap->set(i, i);
		std::cout << "heap[" << i << "]=" << (*heap)[i] << std::endl;
	}
}

