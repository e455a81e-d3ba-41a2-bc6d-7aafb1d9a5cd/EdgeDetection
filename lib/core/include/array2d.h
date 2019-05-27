#pragma once
#include <vector>
#include <memory>
#include "assert.h"

namespace lib {

    template<typename T>
    class array2d_iterator
    {
    public:
        using difference_type   = std::ptrdiff_t;
        using value_type        = typename std::remove_cv<T>::type;
        using pointer           =  T *;
        using reference = T &;
        using iterator_category = std::random_access_iterator_tag;

    private:
        pointer ptr_;

    public:
        array2d_iterator() : ptr_(nullptr) {}
        array2d_iterator(pointer ptr) : ptr_(ptr) {}

        array2d_iterator(const array2d_iterator& other) : ptr_(other.ptr_) {}

        // Accessing
        reference  operator*() const noexcept
        {
            return *ptr_;
        }

        reference operator->() const noexcept
        {
            return ptr_;
        }

        reference operator[](difference_type distance) const noexcept
        {
            return ptr_[distance];
        }

        // Increment / Decrement
        array2d_iterator& operator++() noexcept
        {
            ++ptr_;
            return *this;
        }

        array2d_iterator operator++(int) noexcept
        {
            auto state = *this;
            ++ptr_;
            return state;
        }

        array2d_iterator& operator--() noexcept
        {
            ++ptr_;
            return *this;
        }

        array2d_iterator operator--(int) noexcept
        {
            auto state = *this;
            ++ptr_;
            return state;
        }

        array2d_iterator& operator+=(difference_type distance) noexcept
        {
            ptr_ += distance;
            return *this;
        }

        array2d_iterator& operator-=(difference_type distance) noexcept
        {
            ptr_ -= distance;
            return *this;
        }

        friend array2d_iterator operator+(array2d_iterator iterator, difference_type distance) noexcept
        {
            return array2d_iterator(iterator.ptr_ + distance);
        }

        friend array2d_iterator operator+(difference_type distance, array2d_iterator iterator) noexcept
        {
            return iterator + distance;
        }

        friend array2d_iterator operator-(array2d_iterator iterator, difference_type distance) noexcept
        {
            return array2d_iterator(iterator.ptr_ - distance);
        }

        friend array2d_iterator operator-(difference_type distance, array2d_iterator iterator) noexcept
        {
            return iterator - distance;
        }

        friend bool operator==(array2d_iterator lhs, array2d_iterator rhs)
        {
            return lhs.ptr_ == rhs.ptr_;
        }

        friend bool operator!=(array2d_iterator lhs, array2d_iterator rhs)
        {
            return lhs.ptr_ != rhs.ptr_;
        }

        friend bool operator<(array2d_iterator lhs, array2d_iterator rhs)
        {
            return lhs.ptr_ < rhs.ptr_;
        }

        friend bool operator>(array2d_iterator lhs, array2d_iterator rhs)
        {
            return lhs.ptr_ > rhs.ptr_;
        }

        friend bool operator<=(array2d_iterator lhs, array2d_iterator rhs)
        {
            return lhs.ptr_ <= rhs.ptr_;
        }

        friend bool operator>=(array2d_iterator lhs, array2d_iterator rhs)
        {
            return lhs.ptr_ >= rhs.ptr_;
        }
    };


    template<typename T, typename Deleter = std::default_delete<T[]>>
    class array2d
    {
    public:
        using iterator = array2d_iterator<T>;
        using const_iterator = array2d_iterator<const T>;

    private:
        class row
        {
            friend class array2d<T, Deleter>; //Only array2d may instantiate this class

        private:
            row(T* data, int ncols) :
                row_data_(data),
                ncols_(ncols) {}

            T* row_data_;
            const int ncols_;

        public:
            const T& operator[](const int col_idx) const
            {
                throw_assert(col_idx < ncols_, "Array has " << ncols_ << " columns, but the requested column index was " << col_idx << ".")
                return row_data_[col_idx];
            }

            T& operator[](const int col_idx)
            {
                throw_assert(col_idx < ncols_, "Array has " << ncols_ << " columns, but the requested column index was " << col_idx << ".")
                return row_data_[col_idx];
            }
        };

        std::unique_ptr<T[], Deleter> data_;
        int nrows_ = 0;
        int ncols_ = 0;

    public:
        array2d() : data_(nullptr), nrows_(0), ncols_(0) {}

        array2d(int rows, int cols) : array2d(rows, cols, std::make_unique<T[]>(static_cast<size_t>(rows* cols)))
        {
        }

        array2d(int rows, int cols, std::unique_ptr<T[], Deleter>&& data) :
            data_(std::move(data)),
            nrows_(rows),
            ncols_(cols)
        {
        }

        array2d(array2d<T, Deleter>&& other) noexcept :
            data_(std::move(other.data_)),
            nrows_(std::move(other.nrows_)),
            ncols_(std::move(other.ncols_)) 
        {
        }

        array2d<T, Deleter>& operator=(array2d<T, Deleter>&& other) noexcept
        {
            data_ = std::move(other.data_);
            nrows_ = std::move(other.nrows_);
            ncols_ = std::move(other.ncols_);
            return *this;
        }

        array2d<T, Deleter>& operator=(array2d<T, Deleter>& other) = delete;

        virtual ~array2d() = default;

        row operator[](const int row_idx)
        {
            throw_assert(row_idx < nrows_, "Array has " << nrows_ << " rows, but the requested row index was " << row_idx << ".")
            return row(data_.get() + static_cast<size_t>(row_idx) * ncols_, ncols_);
        }

        const int rows() const
        {
            return nrows_;
        }

        const int cols() const
        {
            return ncols_;
        }

        constexpr const std::size_t size () const
        {
            return ncols_ * nrows_;
        }

        iterator begin() noexcept
        {
            return iterator(&data_[0]);
        }

        iterator end() noexcept
        {
            return iterator(&data_[size() - 1]);
        }

        const_iterator cbegin() const noexcept
        {
            return const_iterator(&data_[0]);
        }

        const_iterator cend() const noexcept
        {
            return const_iterator(&data_[size() - 1]);
        }

        const_iterator begin() const noexcept
        {
            return cbegin();
        }

        const_iterator end() const noexcept
        {
            return cend();
        }

        T* data() {
            return data_.get();
        }
    };
}