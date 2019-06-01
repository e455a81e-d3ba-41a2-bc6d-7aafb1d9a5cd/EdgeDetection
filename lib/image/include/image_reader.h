#pragma once 

#include "stb.h"
#include <string>
#include <memory>
#include <core/core.h>
#include <iostream>

namespace lib 
{
    template<typename T, typename Deleter>
    void read_image(const std::string& path, array2d<T, Deleter>& image)
    {
        int cols;
        int rows;
        int channels;
        auto raw_data = stbi_load(path.c_str(), &cols, &rows, &channels, 0);

        throw_assert(raw_data != nullptr, "Could not read image.");
        throw_assert(pixel_traits<T>::channels() == channels, "Channel count does not match.");

        std::unique_ptr<T[]> data(reinterpret_cast<T*>(raw_data));
        image = std::move(array2d<T>{rows, cols, std::move(data)});
    }
}
