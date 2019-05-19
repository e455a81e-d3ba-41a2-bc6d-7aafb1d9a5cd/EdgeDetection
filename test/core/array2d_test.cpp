#include <gtest/gtest.h>
#include <memory>
#include <core/core.h>

TEST(array2d, indexing_returns_the_correct_value)
{
    auto data = std::make_unique<float[]>(64);
    data[13] = 123;

    lib::array2d<float> array(8, 8, std::move(data));

    ASSERT_EQ(123, array[1][5]);
}