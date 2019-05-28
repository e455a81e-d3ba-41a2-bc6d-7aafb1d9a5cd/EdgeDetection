#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <image/image.h>
#include <core/core.h>


TEST(image_converter, converts_grayscale_to_grayscale)
{
    lib::array2d<uint8_t> image_data;
    lib::read_image("./TestData/cln1.jpg", image_data);
    auto result = lib::convert<uint8_t>(image_data);
    lib::write_image("./TestResults/converts_grayscale_to_grayscale.jpg", result);
}

TEST(image_converter, converts_color_to_color)
{
    lib::array2d<lib::rgb_pixel_u8> image_data;
    lib::read_image("./TestData/lena.jpg", image_data);
    auto tmp = lib::convert<lib::rgb_pixel<float>>(image_data);
    auto result = lib::convert<lib::rgb_pixel<uint8_t>>(tmp);
    lib::write_image("./TestResults/converts_color_to_color.jpg", result);
}

TEST(image_converter, converts_color_to_grayscale_average)
{
    lib::array2d<lib::rgb_pixel_u8> image_data;
    lib::read_image("./TestData/lena.jpg", image_data);
    auto result = lib::convert<uint8_t, lib::grayscale_mode::Average>(image_data);
    lib::write_image("./TestResults/converts_color_to_grayscale_average.jpg", result);
}

TEST(image_converter, converts_color_to_grayscale_luminosity)
{
    lib::array2d<lib::rgb_pixel_u8> image_data;
    lib::read_image("./TestData/lena.jpg", image_data);
    auto result = lib::convert<uint8_t, lib::grayscale_mode::Luminosity>(image_data);
    lib::write_image("./TestResults/converts_color_to_grayscale_luminosity.jpg", result);
}

TEST(image_converter, converts_color_to_grayscale_lightness)
{
    lib::array2d<lib::rgb_pixel_u8> image_data;
    lib::read_image("./TestData/lena.jpg", image_data);
    auto result = lib::convert<uint8_t, lib::grayscale_mode::Lightness>(image_data);
    lib::write_image("./TestResults/converts_color_to_grayscale_lightness.jpg", result);
}
