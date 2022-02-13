#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

int find_key_length(const std::string &buffer);

std::string xor_decrypt(const std::string &buffer);

int main(int argc, char *argv[])
{
	std::ifstream fin;
	std::ofstream fout;
	
	if (argc < 2)
		return 1;
	
	fin.open(argv[1], std::ios::binary);
	if (fin.fail()) {
		std::cout << "Error opening file" << argv[1] << std::endl;
		return 1;
	}
	
	std::string buffer;
	while (!fin.eof()) {
		char ch = fin.get();
		if (!fin.eof())
			buffer.push_back(ch);
	}
	
	std::cout << find_key_length(buffer) << std::endl;
	
	return 0;
}


int coincidense_for_shift(const std::string &buffer, int shift)
{
	int coincidense = 0;
	for (int bufp = 0; bufp < buffer.length(); bufp++, shift++) {
		if (shift >= buffer.length())
			shift = 0;
		if (buffer.at(bufp) == buffer.at(shift))
			coincidense += 1;
	}
	return coincidense;
}

int find_key_length(const std::string &buffer)
{	
	int PrevShift = 0, guess = 0;
	
	for (int shift = 1; shift < buffer.length() && !guess; shift++) {		
	
		float freq = (float)coincidense_for_shift(buffer, shift)
		/ buffer.length();				
		if (freq > 0.05) {
			if (PrevShift != 0) {
				guess = shift - PrevShift;
			}
			PrevShift = shift;
		}
		
	}
			
	return guess;
}

std::string xor_decrypt(const std::string &buffer)
{
	
	
	
	return "";
}

// e1 = x0 ^ k1		en = xn ^ k1

// e1 ^ en = x0 ^ xn

// y1 =  x0 ^ xn y2 = x1 ^ xn+1 ... yn = xn ^ x2n

