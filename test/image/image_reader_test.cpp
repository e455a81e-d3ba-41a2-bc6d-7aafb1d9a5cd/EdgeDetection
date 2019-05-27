#include <gtest/gtest.h>
#include <image/image.h>
#include <core/core.h>

TEST(image_reader, reads_image)
{
    lib::array2d<lib::rgb_pixel_u8> image_data;
    lib::read_image("./TestData/lena.jpg", image_data);
    lib::write_image("./TestData/cln1_test.jpg", image_data);
    ASSERT_NE(&image_data[0][0], nullptr);
}

