#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <core/core.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>


namespace test_helper
{

    template<std::size_t Height, std::size_t Width, typename T>
    using window = typename lib::sliding_window_view<Height, Width, T>::window;

    template<std::size_t Height, std::size_t Width, typename T>
    bool window_matches(const std::vector<typename T::value_type>& expected, const window<Height, Width, T>& actual)
    {
        for (int j = 0; j < Height; j++)
            for (int i = 0; i < Width; i++)
                if (actual[j][i] != expected[j * Width + i])
                {
                    std::cout << "Expected: '" << expected[j * Width + i] << "' but was '" << actual[j][i] << "'." << std::endl;
                    return false;
                }
        return true;
    }
}

TEST(sliding_window_view, returns_correct_window)
{
    auto data = std::make_unique<int[]>(64);

    lib::array2d<int> array(8, 8, std::move(data));
    std::iota(array.begin(), array.end(), 0);

    auto view = lib::make_sliding_window_view<3, 3>(array);

    auto expect = std::vector<int>
    {   
        0, 1, 2,
        8, 9, 10,
        16, 17, 18 
    };

    ASSERT_TRUE((test_helper::window_matches <3, 3, lib::array2d<int>>(expect, view[0])));

    expect = std::vector<int>
    {   
        45, 46, 47,
        53, 54, 55,
        61, 62, 63
    }; 

    ASSERT_TRUE((test_helper::window_matches <3, 3, lib::array2d<int>>(expect, view[35])));

    expect = std::vector<int>
    {   
        18, 19, 20,
        26, 27, 28,
        34, 35, 36
    }; 

    ASSERT_TRUE((test_helper::window_matches <3, 3, lib::array2d<int>>(expect, view[14])));
}

TEST(sliding_window_view, returns_correct_asymmetrical_window)
{
    auto data = std::make_unique<int[]>(48);

    lib::array2d<int> array(4, 12, std::move(data));
    std::iota(array.begin(), array.end(), 0);

    auto view = lib::make_sliding_window_view<2, 8>(array);

    auto expect = std::vector<int>
    {   
        0, 1, 2, 3, 4, 5, 6, 7,
        12, 13, 14, 15, 16, 17, 18, 19
    };

    ASSERT_TRUE((test_helper::window_matches <2, 8, lib::array2d<int>>(expect, view[0])));

    expect = std::vector<int>
    {   
        28, 29, 30, 31, 32, 33, 34, 35,
        40, 41, 42, 43, 44, 45, 46, 47
    };

    ASSERT_TRUE((test_helper::window_matches <2, 8, lib::array2d<int>>(expect, view[14])));
}

TEST(sliding_window_view, supports_range_based_for)
{
    auto data = std::make_unique<int[]>(64);

    lib::array2d<int> array(8, 8, std::move(data));
    std::iota(array.begin(), array.end(), 0);

    lib::sliding_window_view<3,3,lib::array2d<int>> view{array};

    auto expect = std::vector<int>
    {   
        0, 1, 2, 3, 4, 5,
        8, 9, 10, 11, 12, 13,
        16, 17, 18, 19, 20, 21,
        24, 25, 26, 27, 28, 29,
        32, 33, 34, 35, 36, 37,
        40, 41, 42, 43, 44, 45
    };
    
    auto expected_value_at_position = expect.begin();
    for (auto& it: view) {
        EXPECT_EQ(*expected_value_at_position, it[0][0]);
        expected_value_at_position++;
    }
}
