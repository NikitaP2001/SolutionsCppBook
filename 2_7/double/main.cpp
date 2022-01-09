#include <iostream>
#include <sstream>

union double_format {
        double val;
        struct {
                unsigned long long mantisa : 52;
                unsigned long long bias : 11;
                unsigned long long sign : 1;
        } ff;
};

void print_double(double val)
{
        double_format f_val = { val };
                       
        std::cout << "bias: " << f_val.ff.bias << std::endl
        << "mantisa: " << f_val.ff.mantisa;
}

int main()
{         
        print_double(1234.5678);
        
        exit(0);
}