#pragma once
#include <string>

namespace lib {
    template<typename Deleter>
    void write_image(const std::string& path, array2d<unsigned char, Deleter> image)
    {
        stb::stbi_write_jpg(path.c_str(), image.cols(), image.rows(), 1, image.data(), 90);
    }
}