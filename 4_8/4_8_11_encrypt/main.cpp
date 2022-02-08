#include <iostream>
#include <fstream>
#include <cstring>


void chypter(char *key, std::ifstream &fin, std::ofstream &fout)
{
	char ch;
	bool notEof = true;
	int keylen = std::strlen(key);	
			
	for (int pKey = 0; notEof; pKey++) {	
		ch = fin.get();
				
		if (pKey >= keylen)
			pKey = 0;
		ch ^= key[pKey];
		
		if (notEof = !fin.eof())
			fout.put(ch);
	}
}

int main(int argc, char *argv[])
{	
	std::ifstream fin;
	std::ofstream fout;
	
	if (argc != 4)
		return 1;		
		
	fin.open(argv[2], std::ios::binary);
	if (fin.fail()) {
		std::cout << "error opening file " << argv[2] << std::endl;
		return 1;
	}
	
	fout.open(argv[3], std::ios::binary);
	if (fout.fail()) {
		std::cout << "error opening file " << argv[3] << std::endl;
		return 1;
	}
	
	chypter(argv[1], fin, fout);	

	return 0;
}