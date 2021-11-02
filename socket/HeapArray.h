#pragma once
template<typename T>
class HeapArray {
	int m_leng;
	T* m_pointer;
private:
	HeapArray(int leng);
	HeapArray(const HeapArray<T>& obj);
	virtual ~HeapArray();
	bool construct();
public:
	static HeapArray<T>* NewInstance(int leng);
	T& operator[](int index);
	T operator[](int index)const;
	bool set(int index, T var);
	bool get(int index, T& var);
};

template<typename T>
HeapArray<T>::HeapArray(int leng) {
	m_leng = leng;
	
}

template<typename T>
HeapArray<T>::HeapArray(const HeapArray<T>& obj) {
	m_leng = obj.m_leng;
	m_pointer = new T[m_leng];
	for (int i = 0; i < m_leng; i++) {
		T[i] = obj.m_pointer[i];
	}
}

template<typename T>
bool HeapArray<T>::construct() {
	m_pointer = new T[m_leng];
	return m_pointer != NULL;
}

template <typename T>
HeapArray<T>::~HeapArray() {
	delete[] m_pointer;
}

template <typename T>
HeapArray<T>* HeapArray<T>::NewInstance(int leng) {
	HeapArray<T>* obj = new HeapArray<T>(leng);
	if (!(obj && obj->construct())) {
		delete obj;
		obj = NULL;
	}
	return obj;
}

template <typename T>
T& HeapArray<T>::operator[](int index) {
	return m_pointer[index];
}

template <typename T>
T HeapArray<T>::operator[](int index)const {
	return m_pointer[index];
}

template <typename T>
bool HeapArray<T>::get(int index,T& var) {
	if (index < m_leng) {
		var = m_pointer[index];
		return true;
	}
	return false;
}

template <typename T>
bool HeapArray<T>::set(int index, T var) {
	if (index < m_leng) {
		m_pointer[index] = var;
		return true;
	}
	return false;
}