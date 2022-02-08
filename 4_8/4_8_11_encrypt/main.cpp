#include <iostream>
#include <fstream>


char key[] = "SuperSecretKey";


void chypter(std::ifstream &fin, std::ofstream &fout)
{
	char ch;
	bool notEof = true;	
			
	for (int pKey = 0; notEof; pKey++) {	
		ch = fin.get();
				
		if (pKey >= sizeof(key) - 1)
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
	
	if (argc != 3)
		return 1;
	
	fin.open(argv[1], std::ios::binary);
	fout.open(argv[2], std::ios::binary);
	
	chypter(fin, fout);	

	return 0;
}