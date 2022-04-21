// adjust sheckedPtrTOT to class template, where special situations 
// signals about dinamic error
#include <iostream>
#include <exception>

template<typename T>
class CheckedPtrToT {
	T* p;
	T* array;
	int size;		
	
public:
	CheckedPtrToT(T* p, T* a, int s) : p(p), array(a), size(s) {}
	
	CheckedPtrToT(T* p) : p(p), array(p), size(1) {}
	T* operator++()
	{
		if (p < array || p >= array + (size-1)*sizeof(T))
			throw std::out_of_range("pointer out of range");
		else
			return ++p;
	}
	
	T* operator++(int)
	{
		return operator++();
	}
	
	T* operator--()
	{
		if (p <= array || p > array + (size-1)*sizeof(T))
			throw std::out_of_range("pointer out of range");
		else
			return --p;
	}
	
	T* operator--(int)
	{
		return operator--();
	}
	
	T& operator*()
	{
		if (p < array || p > array + (size-1)*sizeof(T))
			throw std::out_of_range("pointer out of range");
		else
			return *p;
	}
};


int main()
{
	std::string a = "will be copied somewhere";
	std::string arr[100];
	CheckedPtrToT<std::string> p(&arr[0], arr, 100);
	
	try {
		
	std::cout << *p << std::endl;
	
	++p;
	*p = a;
	std::cout << *p << std::endl;
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	
}