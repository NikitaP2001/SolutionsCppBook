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
	srep *p;
	public:
	string(const char*);
	string();
	string(const string &);
	string &operator=(const char*);
	string &operator=(const string&);
	~string();
	char &operator[](int i);
	friend std::ostream& operator<<(std::ostream &, const string &);
	friend std::istream& operator>>(std::istream &, string &);
	friend int operator==(const string &x, const char *s)
	{
		return strcmp(x.p->s, s) == 0;
	}
	friend int operator==(const string &x, const string &y)
	{
		return strcmp(x.p->s, y.p->s) == 0;
	}
	friend int operator!=(const string &x, const char *s)
	{
		return strcmp(x.p->s, s) != 0;
	}
	friend int operator!=(const string &x, const string &y)
	{
		return strcmp(x.p->s, y.p->s) != 0;
	}

struct string_iterator {
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = char;
	using pointer = char*;
	using reference = char&;

	string_iterator(pointer ptr) : s_ptr(ptr) {} 

	reference operator*() const { return *s_ptr; }
	pointer operator->() { return s_ptr; }

	// Prefix increment
	string_iterator& operator++() { s_ptr++; return *this; }

	// Postfix increment
	string_iterator operator++(int) { string_iterator tmp = *this; ++(*this); return tmp; }

	friend bool operator== (const string_iterator& a, const string_iterator& b)
	{ return a.s_ptr == b.s_ptr; }
	friend bool operator!= (const string_iterator& a, string_iterator& b)
	{ return a.s_ptr != b.s_ptr; }
private:
	pointer s_ptr;
};
	string_iterator begin()
	{ 
		return string_iterator(&p->s[0]);
	}

    	string_iterator end()
	{ 
		return string_iterator(&p->s[strlen(p->s)]);
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
}

string::string(const char *s)
{
	p = new srep;
	p->s = new char[strlen(s) + 1];
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
	if (p->n > 1) {
		p->n--;
		p = new srep;
	} else 
		delete[] p->s;
	p->s = new char[strlen(s) + 1];
	strcpy(p->s, s);
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

std::ostream& operator<<(std::ostream &s, const string &x)
{
	return s << x.p->s << " [" << x.p->n << "]" << std::endl;	
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
	if (i < 0 || strlen(p->s) < i)
		error("Invalid index value");
	return p->s[i];
}


int main()
{
	string str;
	while (std::cin) {
		std::cin >> str;
		std::cout << "Range-based for" << std::endl;
		for (auto c : str)
			std::cout << c;
		std::cout << std::endl;
	}
}
