#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <image/image.h>
#include <core/core.h>

#include <vector>
#include <complex>

TEST(dft_test, one_dimensional_dft_returns_correct_result)
{
    using namespace std::complex_literals;
    std::vector<std::complex<double>> test_data{ 1., 1., 1., 1. };
    auto dft_result = lib::dft(test_data);

    std::vector<int> result;
    std::transform(std::begin(dft_result), std::end(dft_result), std::back_inserter(result), [](auto a) { return static_cast<int>(a.real()); });

    EXPECT_THAT(result, ::testing::ElementsAreArray({ 4, 0, 0, 0 }));
}

