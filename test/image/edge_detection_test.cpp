#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <image/image.h>
#include <core/core.h>

TEST(edge_detection, sobel)
{
    lib::array2d<lib::rgb_pixel_u8> image_data;
    lib::read_image("./TestData/lena.jpg", image_data);
    auto tmp = lib::convert<float, lib::grayscale_mode::Luminosity>(image_data);

    auto result = lib::sobel(tmp);

    auto output = lib::convert<uint8_t>(result);
    lib::write_image("./TestResults/edge_detection_sobel.jpg", output);
}
