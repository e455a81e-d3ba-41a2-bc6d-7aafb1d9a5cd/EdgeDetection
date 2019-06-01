#ifndef DEBUG_H
#define DEBUG_H
#include <algorithm>
#include <iostream>

template <typename T>
void printKernel(T& kernel)
{
        std::for_each(kernel.begin(), kernel.end(), 
                [](typename T::value_type& it) {
                        std::for_each(it.begin(), it.end(), [](typename T::value_type::value_type& val) {
                                std::cout << val << " ";
                        });
                        std::cout << std::endl;
                }
        );
}



#endif //DEBUG_H
