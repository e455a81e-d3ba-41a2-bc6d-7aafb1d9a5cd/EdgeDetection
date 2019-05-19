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

    array2d<unsigned char, decltype(&image_deleter)> read_image(const std::string& path)
    {
        int cols;
        int rows;
        int channels;
        auto raw_data = stb::stbi_load(path.c_str(), &cols, &rows, &channels, 0);
        std::unique_ptr<unsigned char[], decltype(&image_deleter)> data(raw_data, image_deleter);
        return array2d<unsigned char, decltype(&image_deleter)>{rows, cols, std::move(data)};
    }
}
