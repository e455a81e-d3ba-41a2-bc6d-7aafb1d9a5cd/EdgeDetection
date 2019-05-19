#pragma once
#include<type_traits>
namespace lib {
    template <typename T>
    struct pixel_traits;

    struct rgb_pixel {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    template <>
    struct pixel_traits<rgb_pixel>
    {
        using channels = std::integral_constant<int, 3>;
    };

    template<typename T>
    struct grayscale_pixel_traits
    {
        using channels = std::integral_constant<int, 1>;
    };

    template<> struct pixel_traits<char> : public grayscale_pixel_traits<char> {};
    template<> struct pixel_traits<int> : public grayscale_pixel_traits<int> {};
    template<> struct pixel_traits<float> : public grayscale_pixel_traits<float> {};
    template<> struct pixel_traits<double> : public grayscale_pixel_traits<double> {};

    template<> struct pixel_traits<unsigned char> : public grayscale_pixel_traits<unsigned char> {};
}
