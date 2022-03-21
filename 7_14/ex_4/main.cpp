#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <iterator>

class string {
	std::vector<char> container;
public:
	string(const char*);
	string(const string &);
	string &operator=(const char*);
	string &operator=(const string&);

	string operator+(const string&);
	string &operator+=(const string&);
	string operator()(int p1, int p2);

	char &operator[](int i);
	friend std::ostream& operator<<(std::ostream &, const string &);
	friend std::istream& operator>>(std::istream &, string &);
	friend int operator==(const string &x, const char *s)
	{
		int i = 0;
		for (char c : x.container) {
			if (s[i] != c || s[i] == '\0')
				return false;
			i += 1;
		}
		if (s[i] == '\0')
			return true;
		else
			return false;
	}
	friend int operator==(const string &x, const string &y)
	{
		return x.container == y.container;
	}
	friend int operator!=(const string &x, const char *s)
	{
		return !(x == s);
	}
	friend int operator!=(const string &x, const string &y)
	{
		return !(x == y);
	}
};

string::string(const string &x)
{
	container = x.container;
}

string::string(const char *s)
{
	container = std::vector<char>(s, s + strlen(s) * sizeof(char));
}

string& string::operator=(const char *s)
{
	container = std::vector<char>(s, s + strlen(s) * sizeof(char));
	return *this;
}

string& string::operator=(const string &x)
{
	container = x.container;
	return *this;
}

string string::operator+(const string& another)
{
	string str(*this);
	str += another;	
	return str;
}

string &string::operator+=(const string& another)
{
	for (char c : another.container) {
		container.push_back(c);
	}
	return *this;
}

/*
string string::operator()(int p1, int p2)
{

}
*/

std::ostream& operator<<(std::ostream &s, const string &x)
{
	auto &cont = x.container;
	std::copy(cont.begin(), cont.end(), std::ostream_iterator<char>(s, ""));
	return s;
}

std::istream& operator>>(std::istream &s, string &x)
{
	char c;
	auto &cont = x.container;
	cont.erase(cont.begin(), cont.end());
	while (s.get(c) && !isspace(c))
		x.container.push_back(c);
	return s;
}

char& string::operator[](int i)
{
	if (i >= container.size())
		throw std::range_error("Index out of range");
	else
		return container[i];
}

int main()
{
	string str("Test string");
	string str1 = str;
	std::cin >> str;
	std::cout << str << std::endl;
	
	string str2 = str + " " + str1;
	std::cout << str2 << std::endl;
}
