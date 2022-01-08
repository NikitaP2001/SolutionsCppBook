#include <iostream>

union pointer_data {
        void *pointer;
        char digits[sizeof(void *)];
} nullp;

int main()
{
        nullp.pointer = NULL;
        
        std::cout << "Nullptr digits: 0x";
        for (int i = 0; i < sizeof(void *); i++)
                std::cout << std::hex << (unsigned int)nullp.digits[i];
        
        exit(0);
}