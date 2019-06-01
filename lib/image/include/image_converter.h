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


    namespace internal 
    {
        template<typename TTo,typename TFrom>
        TTo inverse_map_floating_point(TFrom from) 
        {
            static_assert(std::is_floating_point_v<TFrom>);
            static_assert(std::is_integral_v<TTo>);
            return static_cast<TTo>(from * std::numeric_limits<TTo>::max());
        }

        template<typename TTo,typename TFrom>
        TTo map_floating_point(TFrom from) 
        {
            static_assert(std::is_integral_v<TFrom>);
            static_assert(std::is_floating_point_v<TTo>);
            // linear mapping from [0 numeric_limits<TFrom>::max] to [0.0 1.0]
            return static_cast<TTo>(from) / static_cast<TTo>(std::numeric_limits<TFrom>::max());
        }

        template<typename TTo, grayscale_mode Mode, typename TFrom>
        TTo convert_to_grayscale(TFrom from)
        {
            static_assert(Mode != grayscale_mode::None, "No grayscale mode chosen.");

            if constexpr (Mode == grayscale_mode::Average)
            {
                return static_cast<TTo>((from.r + from.g + from.b) / static_cast<TTo>(3));
            }
            if constexpr (Mode == grayscale_mode::Lightness)
            {
                return static_cast<TTo>((std::max({ from.r, from.g, from.b }) + std::min({ from.r, from.g, from.b })) / static_cast<TTo>(2));
            }
            if constexpr (Mode == grayscale_mode::Luminosity)
            {
                return static_cast<TTo>(0.21 * from.r + 0.72 * from.g + 0.07 * from.b);
            }
        }


        template<typename TTo, grayscale_mode Mode, typename TFrom>
        inline TTo convert(const TFrom& from)
        {
            if constexpr (pixel_traits<TFrom>::is_rgb())
            {
                if constexpr (pixel_traits<TFrom>::channels() == 3 && pixel_traits<TTo>::channels() == 3)
                {
                    auto result = TTo{};

                    if constexpr (std::is_integral_v<typename pixel_traits<TFrom>::channel_type> && std::is_floating_point_v<typename pixel_traits<TTo>::channel_type>) 
                    {
                        result.r = map_floating_point<typename pixel_traits<TTo>::channel_type>(from.r);
                        result.g = map_floating_point<typename pixel_traits<TTo>::channel_type>(from.g);
                        result.b = map_floating_point<typename pixel_traits<TTo>::channel_type>(from.b);

                    }
                    else if constexpr (std::is_floating_point_v<typename pixel_traits<TFrom>::channel_type> && std::is_integral_v<typename pixel_traits<TTo>::channel_type>)
                    {
                        result.r = inverse_map_floating_point<typename pixel_traits<TTo>::channel_type>(from.r);
                        result.g = inverse_map_floating_point<typename pixel_traits<TTo>::channel_type>(from.g);
                        result.b = inverse_map_floating_point<typename pixel_traits<TTo>::channel_type>(from.b);
                    }
                    else 
                    {
                        result.r = static_cast<typename pixel_traits<TTo>::channel_type>(from.r);
                        result.g = static_cast<typename pixel_traits<TTo>::channel_type>(from.g);
                        result.b = static_cast<typename pixel_traits<TTo>::channel_type>(from.b);
                    }

                    return result;
                }

                if constexpr (pixel_traits<TFrom>::channels() == 3 && pixel_traits<TTo>::channels() == 1)
                {
                    if constexpr (std::is_integral_v<typename pixel_traits<TFrom>::channel_type> && std::is_floating_point_v<TTo>) 
                    {
                        return map_floating_point<TTo>(convert_to_grayscale<typename pixel_traits<TFrom>::channel_type, Mode>(from));
                    }
                    else 
                    {
                        return convert_to_grayscale<TTo, Mode>(from);
                    }
                }
            }

            if constexpr (pixel_traits<TFrom>::channels::value == 1 && pixel_traits<TTo>::channels::value == 1)
            {
                if constexpr ( std::is_integral_v<TFrom> && std::is_floating_point_v<TTo>) 
                {
                    return map_floating_point<TTo>(from);
                } 
                else if constexpr (std::is_floating_point_v<TFrom> && std::is_integral_v<TTo>)
                {
                    return inverse_map_floating_point<TTo>(from);
                }
                else
                {
                    return static_cast<TTo>(from);
                }
            }
        }
    }


    template<typename TTo, grayscale_mode mode = grayscale_mode::None, typename TFrom, typename Deleter>
    array2d<TTo> convert(const array2d<TFrom, Deleter>& input)
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
