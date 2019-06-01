#pragma once
#include <array>
#include <utility>

namespace lib 
{
    template<std::size_t Height, std::size_t Width, typename T>
    struct kernel
    {
        std::array<T, Height* Width> values;
    };

    template <std::size_t Height, std::size_t Width, typename T, typename... Params>
    constexpr kernel<Height, Width, T> make_kernel(Params&&... args)
    {
        auto output = kernel<Height, Width, T>();
        output.values = std::array<T, Height * Width>{static_cast<T>(std::forward<Params>(args))...};
        return output;
    }


    template <typename T>
    constexpr auto sobel_h = make_kernel<3,3, T>(
        1, 0, -1, 
        2, 0, -2,
        1, 0, -1
    );

    template <typename T>
    constexpr auto sobel_v = make_kernel<3,3, T>(
        1, 2, 1,
        0, 0, 0,
       -1,-2,-1
    );

    template <typename T>
    constexpr auto identity = make_kernel<3,3, T>(
        0, 0, 0, 
        0, 1, 0,
        0, 0, 0
    );

    template <std::size_t Height, std::size_t Width, typename T, std::size_t... N>
    constexpr const typename T::value_type col_fold(const typename sliding_window_view<Height, Width, T>::window_type& window, const kernel<Height, Width, typename T::value_type>& kernel, std::size_t row_idx, std::index_sequence<N...>)
    {

        return ((window[row_idx][N] * kernel.values[row_idx * Width + N]) + ...);
    }

    template <std::size_t Height, std::size_t Width, typename T, std::size_t... N>
    constexpr const typename T::value_type row_fold(const typename sliding_window_view<Height, Width, T>::window_type& window, const kernel<Height, Width, typename T::value_type>& kernel, std::index_sequence<N...>)
    {
        return (col_fold<Height, Width, T>(window, kernel, N, std::make_index_sequence<Width>{}) + ...);
    }

    template<std::size_t Height, std::size_t Width, typename T>
    array2d<T> convolve(const array2d<T>& input, const kernel<Height, Width, T>& convolution_kernel)
    {
        static_assert(Height % 2 == 1, "Height must be an uneven number.");
        static_assert(Width % 2 == 1, "Width must be an uneven number.");

        auto result = array2d<typename std::remove_const<T>::type>(input.rows() - (Height - 1), input.cols() - (Width - 1));

        auto result_iterator = result.begin();
        for (auto& it : lib::make_sliding_window_view<Height, Width>(input))
        {
            *result_iterator = row_fold<Height, Width, const array2d<T>>(it, convolution_kernel, std::make_index_sequence<Height>{});
            result_iterator++;
        }
        return result;
    }
}