#include <stdio.h>
#include <vector>
#include "arithmetic_op.h"

int main()
{
    std::vector<std::string> test_lst;
    test_lst.push_back("1+2+3+4");
    test_lst.push_back("1+2*3");
    test_lst.push_back("1+2*3+4");
    test_lst.push_back("1+4/2+3");
    test_lst.push_back("4-3*(1+2)");
    test_lst.push_back("4-3*(1+2)+1");
    test_lst.push_back("4-3*((1+2)+1");
    test_lst.push_back("4--3*(1+2)+1");
    test_lst.push_back("4-3*((1+2)*2+3)");

    for (size_t i = 0; i < test_lst.size(); i++)
    {
        std::string debug_str;
        printf("%s\n%s=%lld\n=========================\n", 
            debug_str.c_str(), test_lst[i].c_str(), arithmetic(test_lst[i], &debug_str));
    }

    return 0;
}
