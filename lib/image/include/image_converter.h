#pragma once
#include <core/core.h>
#include <algorithm>

namespace lib {

    enum class grayscale_mode
    {
        None,
        Average,
        Luminosity,
        Lightness
    };


    namespace internal {

        template<typename TTo, grayscale_mode mode, typename TFrom>
        TTo convert(const TFrom& from)
        {
            if constexpr (pixel_traits<TFrom>::is_rgb())
            {
                if constexpr (pixel_traits<TFrom>::channels() == 3 && pixel_traits<TTo>::channels() == 3)
                {
                    auto result = TTo{};
                    result.r = static_cast<typename pixel_traits<TTo>::channel_type>(from.r);
                    result.g = static_cast<typename pixel_traits<TTo>::channel_type>(from.g);
                    result.b = static_cast<typename pixel_traits<TTo>::channel_type>(from.b);
                    return result;
                }

                if constexpr (pixel_traits<TFrom>::channels() == 3 && pixel_traits<TTo>::channels() == 1)
                {

                    static_assert(mode != grayscale_mode::None, "No grayscale mode chosen.");

                    if constexpr (mode == grayscale_mode::Average)
                    {
                        return static_cast<TTo>((from.r + from.g + from.b) / static_cast<TTo>(3));
                    }
                    if constexpr (mode == grayscale_mode::Lightness)
                    {
                        return static_cast<TTo>((std::max({ from.r, from.g, from.b }) + std::min({ from.r, from.g, from.b })) / static_cast<TTo>(2));
                    }
                    if constexpr (mode == grayscale_mode::Luminosity)
                    {
                        return static_cast<TTo>(0.21 * from.r + 0.72 * from.g + 0.07 * from.b);
                    }
                }
            }

            if constexpr (pixel_traits<TFrom>::channels::value == 1 && pixel_traits<TTo>::channels::value == 1)
            {
                return static_cast<TTo>(from);
            }
        }
    }


    template<typename TTo, grayscale_mode mode = grayscale_mode::None, typename TFrom>
    array2d<TTo> convert(const array2d<TFrom>& input)
    {
        auto output = array2d<TTo>(input.rows(), input.cols());

        array2d<TFrom>::const_iterator in;
        array2d<TTo>::iterator out;
        for (in = input.cbegin(), out = output.begin();
            in < input.cend(), out < output.end();
            in++, out++)
        {
            *out = internal::convert<TTo, mode, TFrom>(*in);
        }
        return output;
    }
}
