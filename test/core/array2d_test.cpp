#include <gtest/gtest.h>
#include <memory>
#include <core/core.h>
#include <algorithm>

TEST(array2d, indexing_returns_the_correct_value)
{
    auto data = std::make_unique<float[]>(64);
    data[13] = 123;

    lib::array2d<float> array(8, 8, std::move(data));

    ASSERT_EQ(123, array[1][5]);
}

TEST(array2d, iterator_works)
{
    auto data = std::make_unique<float[]>(64);
    std::fill(&data[0], &data[63], 123.f);
    lib::array2d<float> array(8, 8, std::move(data));

    auto counter = 1;
    for (const auto& it : array) {
        ASSERT_EQ(123, it);
        counter++;
    }
    ASSERT_EQ(64, counter);
}