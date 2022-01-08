#include <iostream>


int main()
{
        std::cout << "Sizeof short int: " << sizeof(short) << std::endl
        << "Sizeof int: " << sizeof(int) << std::endl
        << "Sizeof long int: " << sizeof(long) << std::endl
        << "Sizeof long long int: " << sizeof(long long) << std::endl
        << "Sizeof float: " << sizeof(float) << std::endl
        << "Sizeof double: " << sizeof(double) << std::endl
        << "Sizeof long double: " << sizeof(long double) << std::endl
        << "Sizeof ptr: " << sizeof(void *) << std::endl;
        
        exit(0);
}