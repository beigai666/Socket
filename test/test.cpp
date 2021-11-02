// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include "WeakPtr.h"

using namespace std;

class B;	//声明
class A
{
public:
	WeakPtr<B> pb_;
	A() {
		printf("A:%p\n",this);
	}
	~A()
	{
		cout << "A delete\n";
	}
};

class B
{
public:
	SharePtr<A> pa_;
	B() {
		printf("B:%p\n", this);
	}
	~B()
	{
		cout << "B delete\n";
	}
};

class MyException : public exception
{
public:
	MyException();
	~MyException();
	char const* what()const throw (){
		return "C++ Exception";
	}
private:

};

MyException::MyException()
{
}

MyException::~MyException()
{
}


void fun()
{
	SharePtr<A> pa(new A());
	SharePtr<B> pb(new B());
	cout << pb.use_count() << endl;	//1
	cout << pa.use_count() << endl;	//1
	pb->pa_ = pa;
	pa->pb_ = pb;
	cout << pa->pb_.use_count() << endl;
	SharePtr<B> p = pa->pb_.lock();
	cout << pa->pb_.use_count() << endl;
	cout << pb.use_count() << endl;	//2
	cout << pa.use_count() << endl;	//2
	throw MyException();
}

int main()
{
	try
	{
		fun();
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	
	return 0;
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
