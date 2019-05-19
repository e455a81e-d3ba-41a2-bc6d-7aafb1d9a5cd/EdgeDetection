#pragma once 

#include "stb.h"
#include <string>
#include <memory>
#include <core/core.h>
#include <iostream>

namespace lib 
{
    void image_deleter(unsigned char* p)
    {
        stb::stbi_image_free(p);
    }

    template<typename T, typename Deleter>
    void read_image(const std::string& path, array2d<T, Deleter>& image)
    {
        int cols;
        int rows;
        int channels;
        //TODO: find a solution for deleter problem
        auto raw_data = stb::stbi_load(path.c_str(), &cols, &rows, &channels, 0);

        throw_assert(pixel_traits<T>::channels::value == channels, "Channel count does not match.");

        std::unique_ptr<T[]> data(reinterpret_cast<T*>(raw_data));
        image = std::move(array2d<T>{rows, cols*channels, std::move(data)});
    }
}