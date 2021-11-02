#pragma once
#include "Counter.h"
#include "Share.h"

template <class T>
class WeakPtr
{
public:
	WeakPtr();
	WeakPtr(SharePtr<T>const & s);
	WeakPtr(WeakPtr<T>const& w);
	WeakPtr<T>& operator = (SharePtr<T>const& s);
	WeakPtr<T>& operator = (WeakPtr<T>const& w);
	SharePtr<T> lock();
	int use_count();
	bool expired();
	~WeakPtr();
	friend class SharePtr<T>;
private:
	T* ptr;
	Counter* cnt;

protected:
	void release();

};

template <class T>
WeakPtr<T>::WeakPtr()
{
	ptr = NULL;
	cnt = NULL;
}

template <class T>
WeakPtr<T>::WeakPtr(SharePtr<T>const& s):ptr(s.ptr),cnt(s.cnt) {
	//cout << "WeakPtr con SharePtr" << endl;
	s.cnt->weak_count++;
}

template <typename T>
WeakPtr<T>::WeakPtr(WeakPtr<T>const& w) :ptr(w.ptr),cnt(w.cnt){
	//cout << "WeakPtr con WeakPtr" << endl;
	w.cnt->weak_count++;
}

template <typename T>
WeakPtr<T>&  WeakPtr<T>::operator=(SharePtr<T>const& s) {
	//cout << "WeakPtr operator= SharePtr" << endl;
	release();
	s.cnt->weak_count++;
	cnt = s.cnt;
	ptr = s.ptr;
	return *this;
}

template <typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>const& w) {
	//cout << "WeakPtr operator= WeakPtr" << endl;
	if (this != &w) {
		release();
		w.cnt->weak_count++;
		cnt = w.cnt;
		ptr = w.ptr;
	}
	return *this;
}

template <typename T>
SharePtr<T> WeakPtr<T>::lock() {
	return SharePtr<T>(*this);
}

template <class T>
WeakPtr<T>::~WeakPtr()
{
	release();
}

template <typename T>
bool WeakPtr<T>::expired() {
	if (cnt) {
		if (cnt->share_count > 1) {
			return false;
		}
	}
	return true;
}

template <typename T>
void WeakPtr<T>::release() {
	if (cnt) {
		cnt->weak_count--;
		if ((cnt->weak_count) < 1 && (cnt->share_count < 1)) {
			delete cnt;
			cnt = NULL;
		}
	}
}

template <typename T>
int WeakPtr<T>::use_count() {
	if (cnt) {
		return cnt->share_count;
	}
	return 0;
}