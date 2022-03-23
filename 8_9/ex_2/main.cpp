#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <iterator>

template<typename T>
class string {
	std::vector<T> container;
public:
	string(const T*);
	string() { }
	string(const string &);
	string &operator=(const T*);
	string &operator=(const string&);

	string operator+(const string&);
	string &operator+=(const string&);
	string operator()(int p1, int p2);

	char &operator[](int i);
	friend std::ostream& operator<<(std::ostream &s, const string &x)
	{
		auto &cont = x.container;
		std::copy(cont.begin(), cont.end(), std::ostream_iterator<T>(s, ""));
		return s;
	}

	friend std::istream& operator>>(std::istream &s, string &x)
	{
		T c;
		auto &cont = x.container;
		cont.erase(cont.begin(), cont.end());
		while (s.get(c) && !isspace(c))
			x.container.push_back(c);
		return s;
	}

	friend int operator==(const string &x, const T *s)
	{
		int i = 0;
		for (T c : x.container) {
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
	friend int operator!=(const string &x, const T *s)
	{
		return !(x == s);
	}

	friend int operator!=(const string &x, const string &y)
	{
		return !(x == y);
	}
};

template<typename T> string<T>::string(const string &x)
{
	container = x.container;
}

template<typename T> string<T>::string(const T *s)
{
	container = std::vector<T>(s, s + strlen(s) * sizeof(T));
}

template<typename T>
string<T>& string<T>::operator=(const T *s)
{
	container = std::vector<T>(s, s + strlen(s) * sizeof(T));
	return *this;
}

template<typename T>
string<T>& string<T>::operator=(const string &x)
{
	container = x.container;
	return *this;
}

template<typename T>
string<T> string<T>::operator+(const string& another)
{
	string str(*this);
	str += another;
	return str;
}

template<typename T>
string<T> &string<T>::operator+=(const string& another)
{
	for (T c : another.container) {
		container.push_back(c);
	}
	return *this;
}

/*
string string::operator()(int p1, int p2)
{

}
*/

template<typename T>
char& string<T>::operator[](int i)
{
	if (i >= container.size())
		throw std::range_error("Index out of range");
	else
		return container[i];
}


int main()
{
	string<char> str;
	std::cin >> str;
	std::cout << "Read: " << str << std::endl;
}
