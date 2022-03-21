#include <iostream>
#include <fstream>
#include <iterator>


int main(int argc, char *argv[])
{
	while (--argc > 0) {
		std::fstream fs(*++argv, std::ios::in | std::ios::binary);
		if (fs) {
			std::cout << fs.rdbuf();
		} else
			std::cerr << "file " << *argv << " not found" << std::endl;
	}
	exit(0);
}
