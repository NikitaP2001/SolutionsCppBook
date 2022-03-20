#include <iostream>
#include <sstream>
#include <iterator>
#include <cstddef>
#include <string.h>

class string {
	struct srep {
		char *s;
		int n;
		srep() { n = 1; }
	};
	int beg, end;
	srep *p;
	public:
	string(const char*);
	string();
	string(const string &);
	string &operator=(const char*);
	string &operator=(const string&);

	string operator+(const string&);
	string &operator+=(const string&);
	string operator()(int p1, int p2);

	~string();
	char &operator[](int i);
	friend std::ostream& operator<<(std::ostream &, const string &);
	friend std::istream& operator>>(std::istream &, string &);
	friend int operator==(const string &x, const char *s)
	{
		return strncmp(&x.p->s[x.beg], s, x.end - x.beg + 1) == 0;
	}
	friend int operator==(const string &x, const string &y)
	{
		int len = (x.end - x.beg > y.end - y.beg) ? 
			y.end - y.beg + 1 : x.end - x.beg + 1;
		return strncmp(&x.p->s[x.beg], &y.p->s[y.beg], len) == 0; 
	}
	friend int operator!=(const string &x, const char *s)
	{
		return strncmp(&x.p->s[x.beg], s, x.end - x.beg + 1) != 0;
	}
	friend int operator!=(const string &x, const string &y)
	{
		int len = (x.end - x.beg > y.end - y.beg) ? 
			y.end - y.beg + 1 : x.end - x.beg + 1;
		return strncmp(&x.p->s[x.beg], &y.p->s[y.beg], len) != 0; 
	}
};

string::string()
{
	p = new srep();
	p->s = 0;
}

string::string(const string &x)
{
	x.p->n++;
	p = x.p;
	beg = x.beg;
	end = x.end;
}

string::string(const char *s)
{
	p = new srep;
	p->s = new char[(end = strlen(s)) + 1];
	end -= 1;
	beg = 0;
	strcpy(p->s, s);
}

string::~string()
{
	if (--p->n == 0) {
		delete[] p->s;
		delete p;
	}
}

string& string::operator=(const char *s)
{
	strncpy(&p->s[beg], s, end - beg + 1);
	return *this;
}

string& string::operator=(const string &x)
{
	x.p->n++;
	if (--p->n == 0) {
		delete[] p->s;
		delete p;
	}
	p = x.p;
	return *this;
}

string string::operator+(const string& another)
{
	string str(this->p->s);
	str += another;
	return str;
}

string &string::operator+=(const string& another)
{
	char *temp = new char[strlen(p->s) + another.end - another.beg + 2];
	strncpy(temp, p->s, this->end + 1);
	strncat(temp, &another.p->s[another.beg], another.end - another.beg + 1);
	strcat(temp, &p->s[this->end+1]);
	int beg = this->beg, end = this->end + another.end - another.beg + 1; 
	if (--p->n == 0) {
		delete[] p->s;
		delete p;
	}
	p = new srep;
	this->beg = beg;
	this->end = end;
	p->s = temp;
	return *this;
}

string string::operator()(int p1, int p2)
{
	if (p1 > p2) {
		int temp = p1;
		p1 = p2;
		p2 = temp;
	}
	p1 = (p1 > end - beg + 1) ? end - beg + 1 : p1;
	p2 = (p2 > end - beg + 1) ? end - beg + 1 : p2;
	string sub;
	sub.p = p;
	p->n += 1;
	sub.beg = beg + p1; 
	sub.end = beg + p2;
	return sub;
}

std::ostream& operator<<(std::ostream &s, const string &x)
{
	char *out = new char[x.end - x.beg + 2];
	strncpy(out, &x.p->s[x.beg], x.end - x.beg + 1);
	s << out << " [" << x.p->n << "]" << std::endl;	
	delete[] out;
	return s;
}

std::istream& operator>>(std::istream &s, string &x)
{
	char buf[256];
	s >> buf;

	x = buf;
	std::cout <<  "echo: " << x << std::endl;
	return s;
}

void error(const char *p)
{
	std::cerr << p << std::endl;
	exit(1);
}

char& string::operator[](int i)
{
	if (i < 0 || end - beg < i)
		error("Invalid index value");
	return p->s[beg + i];
}


int main()
{
	string str = "One two three";
	str(4, 6) = "three";
	std::cout << str << std::endl;
	str += " five";
	std::cout << str << std::endl;
	exit(0);
}
