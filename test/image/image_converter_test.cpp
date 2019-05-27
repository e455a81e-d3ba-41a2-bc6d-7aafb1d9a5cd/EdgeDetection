#include <gtest/gtest.h>
#include <image/image.h>
#include <core/core.h>

TEST(image_converter, converts_grayscale)
{
    lib::array2d<uint8_t> image_data;
    lib::read_image("./TestData/cln1.jpg", image_data);
    auto result = lib::convert<float>(image_data);
    ASSERT_NE(&result[0][0], nullptr);
}

TEST(image_converter, converts_color_to_color)
{
    lib::array2d<lib::rgb_pixel_u8> image_data;
    lib::read_image("./TestData/lena.jpg", image_data);
    auto result = lib::convert<lib::rgb_pixel<float>>(image_data);
    ASSERT_NE(&result[0][0], nullptr);
}
