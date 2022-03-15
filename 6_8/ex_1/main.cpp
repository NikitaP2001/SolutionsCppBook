#include <iostream>
using namespace std;

class base {
public:
	virtual void iam() { cout << "base" << endl; }
};

class D1 : public base {
	public:	
	void iam() { cout << "D1" << endl; }
};

class D2 : public base {
	public:
	void iam() { cout << "D2" << endl; }
};

int main()
{
	D1 d1;
	D2 d2;
	d1.iam();
	d2.iam();

	base *pb = &d1;
	pb->iam();

	pb = &d2;
	pb->iam();
}
