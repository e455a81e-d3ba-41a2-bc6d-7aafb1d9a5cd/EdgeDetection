#pragma once
#include<type_traits>
#include<iterator>
#include<functional>

namespace lib
{

    template <std::size_t Height, std::size_t Width, typename T>
    class sliding_window_view;

    template <typename T, typename TContainer>
    class sliding_window_iterator
    {
        using window = typename TContainer::window_type;

        using difference_type   = std::size_t;
        using reference         = window &;
        using pointer           = window *;
        using value_type        = window;
        using iterator_category = std::bidirectional_iterator_tag;

        difference_type idx_;
        TContainer& view_;

    public:
        sliding_window_iterator(difference_type idx, TContainer& view) : idx_(idx), view_(view) { }

        sliding_window_iterator(const sliding_window_iterator& other) : idx_(other.idx_), view_(other.view_) { }
        sliding_window_iterator(sliding_window_iterator&& other) : idx_(std::move(other.idx_)), view_(std::move(other.view_)) { other.idx_ = 0; }


        value_type operator*()
        {
            return view_[idx_];
        }

        sliding_window_iterator& operator++() noexcept
        {
            ++idx_;
            return *this;
        }

        sliding_window_iterator operator++(int) noexcept
        {
            auto state = *this;
            ++idx_;
            return state;
        }

        sliding_window_iterator& operator--() noexcept
        {
            --idx_;
            return *this;
        }

        sliding_window_iterator operator--(int) noexcept
        {
            auto state = *this;
            --idx;
            return state;
        }

        friend bool operator!=(sliding_window_iterator lhs, sliding_window_iterator rhs)
        {
            return (lhs.idx_ != rhs.idx_);
        }
    };

    template <std::size_t Height, std::size_t Width, typename T>
    class sliding_window_view
    {
        static_assert(Height != 0 && Width != 0, "Height and width can not be zero.");
    public:
        class window
        {
            using value_type = typename T::value_type;
            class window_row
            {
                typename T::iterator iterator_;

            public:
                window_row(typename T::iterator&& iterator) : iterator_(std::move(iterator)) {}

                typename T::value_type& operator[](std::size_t col)
                {
                    return *(iterator_ + col);
                }

                const typename T::value_type& operator[](std::size_t col) const
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
                iterator += idx_ + row * container_.cols();
                return window_row(std::move(iterator));
            }

            const window_row operator[](std::size_t row) const
            {
                auto iterator = container_.begin();
                iterator += idx_ + row * container_.cols();
                return window_row(std::move(iterator));
            }
        };

        using iterator = sliding_window_iterator<T, sliding_window_view>;
        using const_iterator = sliding_window_iterator<T, sliding_window_view>;
        using value_type = T;
        using window_type = window;

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

        iterator begin()
        {
            return iterator(0, *this);
        }

        iterator end()
        {
            return iterator(size(), *this);
        }

        const_iterator cbegin() const
        {
            return iterator(0, *this);
        }

        const_iterator cend() const
        {
            return iterator(size(), *this);
        }

        const_iterator begin() const
        {
            return cbegin();
        }

        const_iterator end() const
        {
            return cend();
        }

    };

    template<std::size_t Height, std::size_t Width, typename T>
    sliding_window_view<Height, Width, T> make_sliding_window_view(T& container) 
    {
        return sliding_window_view<Height, Width, T>(container);
    }
}
