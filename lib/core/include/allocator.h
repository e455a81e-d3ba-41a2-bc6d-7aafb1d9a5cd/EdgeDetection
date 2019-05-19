#pragma once
#include <cstdlib>
#include <new>

namespace lib {
    template <typename T>
    class malloc_allocator
    {
    public:
        using value_type = T;

        malloc_allocator() noexcept = default;
        template <typename U>
        malloc_allocator(const malloc_allocator<U>&) noexcept = default;

        T* allocate(const size_t n) const {
            if (n == 0)
                return nullptr;

            void* const pt = malloc(n * sizeof(T));

            if (pt == nullptr)
                throw std::bad_alloc();

            return static_cast<T*>(pt);
        }

        void deallocate(T * const p, const size_t n) const {
            free(p);
        }
    }


    template <class T, class U>
    constexpr bool operator== (const malloc_allocator<T>&, const malloc_allocator<U>&) noexcept
    {
        return true;
    };

    template <class T, class U>
    constexpr bool operator!= (const malloc_allocator<T>&, const malloc_allocator<U>&) noexcept {
        return !(*this == other);
    }
}
