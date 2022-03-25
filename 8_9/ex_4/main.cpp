#include <string>
#include <iostream>
#include "Map.hpp"

int main()
{
	Map<std::string, int> count("", 0);
	std::string word;
	while(std::cin >> word) count[word]++;
	for (Mapiter<std::string, int> p = count.first(); p; p++)
		std::cout << p.value() << '\t' << p.key() << std::endl;
	return 0;
}
