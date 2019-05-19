#include <gtest/gtest.h>
#include <image/image.h>
#include <core/core.h>

TEST(image_reader, reads_image)
{
    auto image_data = lib::read_image("./TestData/cln1.jpg");
    ASSERT_NE(&image_data[0][0], nullptr);
}

