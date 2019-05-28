#pragma once
#include <string>
#include "pixel.h"

namespace lib {
    template<typename T, typename Deleter>
    void write_image(const std::string& path, array2d<T, Deleter>& image)
    {
        static_assert(pixel_traits<T>::channel_size() == 1);
        stbi_write_jpg(path.c_str(), image.cols(), image.rows(), pixel_traits<T>::channels(), image.data(), 90);
    }
}