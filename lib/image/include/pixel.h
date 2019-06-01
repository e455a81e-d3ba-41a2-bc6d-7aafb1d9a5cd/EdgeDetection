#pragma once
#include<type_traits>
namespace lib {
    template <typename T>
    struct pixel_traits;

    template<typename T>
    struct rgb_pixel {
        T r;
        T g;
        T b;
    };

    typedef rgb_pixel<uint8_t> rgb_pixel_u8;

    template <typename T>
    struct pixel_traits<rgb_pixel<T>>
    {
        using is_rgb = std::bool_constant <true>;
        using is_grayscale = std::bool_constant <false>;
        using channels = std::integral_constant<int, 3>;
        using channel_type = T;
        using channel_size =  std::integral_constant<std::size_t, sizeof T>;
    };

    template<typename T>
    struct grayscale_pixel_traits
    {
        using is_rgb = std::bool_constant <false>;
        using is_grayscale = std::bool_constant <true>;
        using channels = std::integral_constant<int, 1>;
        using channel_type = T;
        using channel_size =  std::integral_constant<std::size_t, sizeof T>;
    };

    template<> struct pixel_traits<char> : public grayscale_pixel_traits<char> {};
    template<> struct pixel_traits<int> : public grayscale_pixel_traits<int> {};
    template<> struct pixel_traits<float> : public grayscale_pixel_traits<float> {};
    template<> struct pixel_traits<double> : public grayscale_pixel_traits<double> {};

    template<> struct pixel_traits<unsigned char> : public grayscale_pixel_traits<unsigned char> {};
}
