#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <image/image.h>
#include <core/core.h>
#include <numeric>

#include <vector>

TEST(convolve, calculates_convolution)
{
    lib::array2d<int> array(8, 8);
    std::iota(array.begin(), array.end(), 0);

    auto result = lib::convolve(array, lib::kernels::identity<int>);

    for (int j = 0; j < result.rows(); j++)
    {
        for (int i = 0; i < result.cols(); i++)
        {
            std::cout << result[j][i] << " ";
        }
        std::cout << std::endl;
    }
}

TEST(convolve, convolve_image_identity)
{
    lib::array2d<lib::rgb_pixel_u8> image_data;
    lib::read_image("./TestData/lena.jpg", image_data);
    auto tmp = lib::convert<float, lib::grayscale_mode::Luminosity>(image_data);
    auto result = lib::convolve(tmp, lib::kernels::identity<float>);
    auto output = lib::convert<uint8_t>(result);
    lib::write_image("./TestResults/convolution_identity.jpg", output);
}

TEST(convolve, convolution_result_is_correct)
{
    lib::array2d<int> array(3, 3);
    std::iota(array.begin(), array.end(), 1);

    auto result = lib::convolve(array, lib::kernels::sobel_v<int>);

    ASSERT_EQ(-24, result[0][0]);
}

TEST(convolve, convolution_works_with_custom_kernel)
{
    lib::array2d<int> array(3, 3);
    std::iota(array.begin(), array.end(), 1);

    auto result = lib::convolve(array, lib::kernels::sobel_v<int>);

    ASSERT_EQ(-24, result[0][0]);
}
