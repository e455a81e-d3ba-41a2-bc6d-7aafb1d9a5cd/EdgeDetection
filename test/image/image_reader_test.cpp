#include <gtest/gtest.h>
#include <image/image.h>
#include <core/core.h>

TEST(image_reader, reads_image)
{
    lib::array2d<unsigned char> image_data;
    lib::read_image("./TestData/cln1.jpg", image_data);
    ASSERT_NE(&image_data[0][0], nullptr);
}

