#pragma once
template <class T, int N>
class Array {
	T array[N];
public:
	Array();
	virtual ~Array();
	int leng();
	T& operator [](int index);
	T operator[](int index)const;
	bool set(int index, T var);
	bool get(int index, T& var);
};

template <class T,int N>
Array<T, N>::Array() {
	
	
}

template <class T,int N>
bool Array<T, N>::get(int index, T& var) {
	if (index > N) {
		return false;
	}
	var = array[index];
	return true;
}

template <class T,int N>
bool Array<T, N>::set(int index, T var) {
	if (index > N) {
		return false;
	}
	array[index] = var;
}

template <class T,int N>
T& Array<T, N>::operator[](int index) {
	return array[index];
}

template <typename T, int N>
T Array<T, N>::operator[](int index)const {
	return array[index];
}

template <class T,int N>
Array<T, N>::~Array() {

}