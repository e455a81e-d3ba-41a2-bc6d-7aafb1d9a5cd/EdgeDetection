#pragma once
#include<type_traits>
#include<iterator>

namespace lib
{

    template <std::size_t Height, std::size_t Width, std::size_t Stride, typename T>
    class sliding_window_view
    {
        static_assert(Height > 0 && Width > 0, "Height and width must be above zero.");

        class window
        {

            class window_row
            {
                typename T::iterator iterator_;

            public:
                window_row(typename T::iterator&& iterator) : iterator_(std::move(iterator)) {}

                typename T::value_type& operator[](std::size_t col)
                {
                    return *(iterator_ + col);
                }
            };

            T& container_;
            std::size_t idx_;

        public:
            window(T& container, const std::size_t idx) : container_(container), idx_(idx)
            {
            }

            window_row operator[](std::size_t row)
            {
                auto iterator = container_.begin();
                iterator += idx_ + row * container_.rows();
                return window_row(std::move(iterator));
            }

            const window_row operator[](std::size_t row) const
            {
                auto iterator = container_.begin();
                iterator += idx_ + row * container_.rows();
                return window_row(std::move(iterator));
            }
        };

    private:
        T& container_;
        std::size_t max_row_;
        std::size_t max_col_;

    public:
        sliding_window_view(T& container) :
            container_(container),
            max_row_(container.rows() - (Height - 1)),
            max_col_(container.cols() - (Width - 1))
        { }

        window operator[](std::size_t idx)
        {
            return window(container_, idx + (idx / (max_col_)) * (Width - 1 ));
        }

        constexpr const std::size_t size() const noexcept
        {
            return max_row_ * max_col_;
        }
    };
}
