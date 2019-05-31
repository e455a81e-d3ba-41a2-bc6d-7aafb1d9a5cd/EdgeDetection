#include <gtest/gtest.h>
#include <memory>
#include <core/core.h>
#include <algorithm>
#include <iostream>

TEST(sliding_window_iterator, sliding_window_iterator_slides_the_window)
{
    auto data = std::make_unique<float[]>(64);

    lib::array2d<float> array(8, 8, std::move(data));

    float x = 0;
    for (auto& it : array) {
        it = x; 
        x++;
    }

    lib::sliding_window_view<3,3, 1, lib::array2d<float>> view{array};
    
    for (int i = 0; i < view.size(); i++) {
        std::cout << view[i][2][2] << " ";
    }
    std::cout << std::endl;
}
