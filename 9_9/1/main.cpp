#include <exception>
#include <iostream>

typedef void(*PFV) ();

template<typename T> 
class STC {
	T old;
	
public:
	STC(T f)
	{
		old = std::set_unexpected(f);		
	}
	~STC()
	{
		std::set_unexpected(old);
	}
};

void g()
{
	throw std::logic_error("Oh shit!");
}

void nag()
{
	std::cerr << "Unhandled exception occured" << std::endl;
}

int main()
{
	STC<void (*)()> xx(&nag);
	try {
		g();
	} catch(std::bad_alloc) {
	}
}