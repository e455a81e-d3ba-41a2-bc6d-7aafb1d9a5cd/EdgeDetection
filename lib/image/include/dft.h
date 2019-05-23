#pragma once
#include <complex>
#include <vector>
#include <cmath>

namespace lib
{
    template<typename T, typename =  std::enable_if_t< std::is_floating_point<T>::value>>
    std::vector<std::complex<T>> dft(const std::vector<std::complex<T>>& input) 
    {
        size_t n = input.size();
        std::vector<std::complex<T>> output(n);
        for (size_t k = 0; k < n; k++)
        {
            for (size_t t = 0; t < n; t++)
            {
                auto angle = 2 * M_PI * t * k / n;
                auto frequency_contribution = std::polar(1., angle);
                output[k] += frequency_contribution;
            }
        }
        return output;
    }
}