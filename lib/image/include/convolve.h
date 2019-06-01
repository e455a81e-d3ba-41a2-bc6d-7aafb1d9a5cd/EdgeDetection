#pragma once
#include <array>
#include <utility>
#include "kernel.h"

namespace lib 
{
    namespace internal {
        // Helper functions for convolve_window_unrolled
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
    }

    // Calculates one convolution without looping over the kernel
    template <std::size_t Height, std::size_t Width, typename T>
    constexpr const typename T::value_type convolve_window_unrolled(
        const typename sliding_window_view<Height, Width, T>::window_type& window,
        const kernel<Height, Width, typename T::value_type>& kernel)
    {
        return internal::row_fold<Height, Width, T>(window, kernel, std::make_index_sequence<Height>{});
    }

    template<std::size_t Height, std::size_t Width, typename T, typename Deleter>
    array2d<T> convolve(const array2d<T, Deleter>& input, const kernel<Height, Width, T>& convolution_kernel)
    {
        static_assert(Height % 2 == 1, "Height must be an uneven number.");
        static_assert(Width % 2 == 1, "Width must be an uneven number.");

        auto result = array2d<typename std::remove_const<T>::type>(input.rows() - (Height - 1), input.cols() - (Width - 1));

        auto result_iterator = result.begin();
        for (auto& it : lib::make_sliding_window_view<Height, Width>(input))
        {
            *result_iterator = convolve_window_unrolled<Height, Width, const array2d<T, Deleter>>(it, convolution_kernel);
            result_iterator++;
        }
        return result;
    }
}