#pragma once
#include <vector>
#include <memory>
#include "assert.h"

namespace lib {

    template<typename T, typename Deleter = std::default_delete<T[]>>
    class array2d
    {
        class row
        {
            friend class array2d<T, Deleter>;

        private:

            row(T* data, int ncols_) :
                row_data_(data),
                ncols_(ncols_) {}

            T* row_data_;
            int ncols_;

        public:
            const T& operator[](const int col_idx) const
            {
                throw_assert(col_idx < ncols_, "Array has " << ncols_ << " columns, but the requested column index was " << col_idx<< ".")
                return row_data_[col_idx];
            }

            T& operator[](const int col_idx)
            {
                throw_assert(col_idx < ncols_, "Array has " << ncols_ << " columns, but the requested column index was " << col_idx<< ".")
                return row_data_[col_idx];
            }
        };

    private:
        std::unique_ptr<T[], Deleter> data_;
        int nrows_;
        int ncols_;
    public:
        array2d(): nrows_(0), ncols_(0) {}

        array2d(const int rows, const int cols) : array2d(rows, cols, std::make_unique<T[]>(static_cast<size_t>(rows* cols)))
        {
        }

        array2d(const int rows, const int cols, std::unique_ptr<T[], Deleter>&& data) :
            data_(std::move(data)),
            nrows_(rows),
            ncols_(cols)
        {
        }

        array2d<T, Deleter>& operator=(array2d<T, Deleter>&& other) noexcept
        {
            data_ = std::move(other.data_);
            nrows_ = std::move(other.nrows_);
            ncols_ = std::move(other.ncols_);
            return *this;
        }

        virtual ~array2d() = default;

        row operator[](const int row_idx)
        {
            throw_assert(row_idx < nrows_, "Array has " << nrows_ << " rows, but the requested row index was " << row_idx << ".")
            return row(data_.get() + static_cast<size_t>(row_idx) * ncols_, ncols_);
        }

        const int rows()
        {
            return nrows_;
        }

        const int cols()
        {
            return ncols_;
        }

        T* data() {
            return data_.get();
        }
    };
}