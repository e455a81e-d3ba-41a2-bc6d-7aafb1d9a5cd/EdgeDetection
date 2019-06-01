#pragma once
#include <core/core.h>
#include <algorithm>
#include "kernel.h"
#include "convolve.h"
#include "pixel.h"

namespace lib
{
    template<typename T, typename Deleter>
    array2d<T> sobel(const array2d<T, Deleter>& input)
    {
        static_assert(pixel_traits<T>::is_grayscale(), "Image must be grayscale.");

        auto result = array2d<typename std::remove_const<T>::type>(input.rows() - 2, input.cols() - 2);
        auto result_iterator = result.begin();

        for (auto& it : lib::make_sliding_window_view<3, 3>(input))
        {
            auto sobel_h_convolution_result = convolve_window_unrolled<3, 3, const array2d<T, Deleter>>(it, kernels::sobel_h<T>);
            auto sobel_v_convolution_result = convolve_window_unrolled<3, 3, const array2d<T, Deleter>>(it, kernels::sobel_v<T>);

            auto magnitude = sqrt(sobel_h_convolution_result * sobel_h_convolution_result +
                sobel_v_convolution_result * sobel_v_convolution_result);

            if constexpr (std::is_floating_point_v<T>)
                * result_iterator = std::clamp(magnitude, T{ 0 }, T{ 1 });
            else
                *result_iterator = std::clamp(magnitude, T{ 0 }, std::numeric_limits<T>::max());

            result_iterator++;
        }
        return result;
    }
}