#include <iostream>
#include <string>
#include <fstream>
#include "./../ex_4/Map.hpp"
#include "./../ex_6/Map.hpp"


void test_lmap()
{
	std::fstream fout("lout.txt", std::ios::out);
	std::fstream fin("bible.txt", std::ios::in);

	listmap::Map<std::string, int> count("", 0);
	std::string word;
	while(fin >> word) count[word]++;
	for (listmap::Mapiter<std::string, int> p = count.first(); p; p++)
		fout << p.value() << '\t' << p.key() << std::endl;

}

void test_tmap()
{
	std::fstream fout("tout.txt", std::ios::out);
	std::fstream fin("bible.txt", std::ios::in);

	tree_map::Map<std::string, int> count("", 0);
	std::string word;
	while(fin >> word) count[word]++;
	for (tree_map::Mapiter<std::string, int> p = count.first(); p; p++)
		fout << p.value() << '\t' << p.key() << std::endl;
}

int main()
{
	test_tmap();

	test_lmap();
}
