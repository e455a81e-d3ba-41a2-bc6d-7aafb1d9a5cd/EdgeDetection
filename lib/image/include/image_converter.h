#pragma once
#include <core/core.h>

namespace lib {
    namespace internal{
        template<typename TTo, typename TFrom>
        TTo convert(const TFrom& from)
        {
            if constexpr ( pixel_traits<TFrom>::is_rgb())
            {
                if constexpr (pixel_traits<TFrom>::channels() == 3 && pixel_traits<TTo>::channels()== 3)
                {
                    auto result = TTo{};
                    result.r = static_cast<typename pixel_traits<TTo>::channel_type>(from.r);
                    result.g = static_cast<typename pixel_traits<TTo>::channel_type>(from.g);
                    result.b = static_cast<typename pixel_traits<TTo>::channel_type>(from.b);
                    return result;
                }
            }
            else if constexpr (pixel_traits<TFrom>::channels::value == 1 && pixel_traits<TTo>::channels::value == 1)
            {
                return static_cast<TTo>(from);
            }
        }
    }


    template<typename TTo, typename TFrom>
    array2d<TTo> convert(const array2d<TFrom>& input) 
    {
        auto output = array2d<TTo>(input.rows(), input.cols());

        array2d<TFrom>::const_iterator in;
        array2d<TTo>::iterator out;
        for (in = input.cbegin(), out = output.begin();
            in < input.cend(), out < output.end();
            in++, out++) 
        {
            *out = internal::convert<TTo>(*in);
        }
        return output;
    }

}
