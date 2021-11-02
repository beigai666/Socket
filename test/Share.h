#pragma once
#include "Counter.h"
#include <iostream>
using namespace std;
template <class T>
class WeakPtr;

template <typename T>
class SharePtr
{
public:
	SharePtr(T* p = 0);
	~SharePtr();
	SharePtr(SharePtr<T>const& s);
	SharePtr(WeakPtr<T>const& w);
	SharePtr<T>& operator =(SharePtr<T>const& s);
	int use_count();
	T& operator *();
	T* operator ->();
private:
	T* ptr;
	Counter* cnt;
	friend class WeakPtr<T>;
protected:
	void release() {
		
		cnt->share_count--;
		if (cnt->share_count < 1) {
			printf("ptr=%p\n", ptr);
			delete ptr;
			if (cnt->weak_count < 1) {
				delete cnt;
				cnt = NULL;
			}
		}
	}
};

template <typename T>
SharePtr<T>::SharePtr(T *p):ptr(p)
{
	cnt = new Counter;
	if (p) {
		cnt->share_count = 1;
	}
}

template<typename T>
int SharePtr<T>::use_count() {
	if (cnt) {
		return cnt->share_count;
	}
	return 0;
}

template <typename T>
SharePtr<T>::SharePtr(SharePtr<T>const& s) {

	s.cnt->share_count++;
	ptr = s.ptr;
	cnt = s.cnt;
	//cout << "copy SharePtr construct" << (s.cnt)->share_count << endl;
}

template <typename T>
SharePtr<T>::SharePtr(WeakPtr<T>const& w) {
	//cout << "in WeakPtr copy con " << endl;
	if (w.cnt->share_count > 0) {
		w.cnt->share_count++;
		ptr = w.ptr;
		cnt = w.cnt;
	}
	else
	{
		ptr = NULL;
		cnt = NULL;
	}
	//cout << "copy WeakPtr  construct" << (w.cnt)->share_count << endl;
}

template<typename T>
SharePtr<T>& SharePtr<T>::operator =(SharePtr<T>const& s) {
	if (this != &s) {
		release();
		s.cnt->share_count++;
		//cout << "assign construct " << (s.cnt)->share_count << endl;
		cnt = s.cnt;
		ptr = s.ptr;
	}
	return *this;
}

template <typename T>
T& SharePtr<T>::operator*() {
	return *ptr;
}

template<typename T>
T* SharePtr<T>::operator->() {
	return ptr;
}
template <typename T>
SharePtr<T>::~SharePtr()
{
	release();
}