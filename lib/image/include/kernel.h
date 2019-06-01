#pragma once
namespace lib
{
    template<std::size_t Height, std::size_t Width, typename T>
    struct kernel
    {
        std::array<T, Height* Width> values;
    };

    template <std::size_t Height, std::size_t Width, typename T, typename... Params>
    constexpr kernel<Height, Width, T> make_kernel(Params&& ... args)
    {
        auto output = kernel<Height, Width, T>();
        output.values = std::array<T, Height* Width>{static_cast<T>(std::forward<Params>(args))...};
        return output;
    }


    namespace kernels
    {
        template <typename T>
        constexpr auto sobel_h = make_kernel<3, 3, T>(
            1, 0, -1,
            2, 0, -2,
            1, 0, -1
            );

        template <typename T>
        constexpr auto sobel_v = make_kernel<3, 3, T>(
            1, 2, 1,
            0, 0, 0,
            -1, -2, -1
            );

        template <typename T>
        constexpr auto identity = make_kernel<3, 3, T>(
            0, 0, 0,
            0, 1, 0,
            0, 0, 0
            );

        template <typename T>
        constexpr auto sharpen = make_kernel<3, 3, T>(
            0, -1, 0,
            -1, 5, -1,
            0, -1, 0
            );

        template <typename T>
        constexpr auto gaussian_blur = make_kernel<3, 3, T>(
            1, 2, 1,
            2, 4, 2,
            1, 2, 1
            );
    }
}