#include "../include/stb.h"
#include <algorithm>


#define STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_WRITE_IMPLEMENTATION

namespace lib::intern {
    template<typename T>
    T* cpp_realloc(T* oldbuf, int oldsize, int newsize) {
        auto newbuf = new T[newsize];
        std::copy_n(oldbuf, std::min(oldsize, newsize), newbuf);
        delete[] oldbuf;
        return newbuf;
    }

    void* cpp_malloc(size_t size) {
        return new char* [size];
    }

    template<typename T>
    void cpp_free(T* p) {
        delete[] p;
    }
}

#define STBI_ASSERT(x)
#define STBI_NO_GIF
#define STBI_MALLOC(size) lib::intern::cpp_malloc(size)
#define STBI_REALLOC_SIZED(oldbuf, oldsize, newsize) lib::intern::cpp_realloc(oldbuf, oldsize, newsize)
#define STBI_FREE(p) lib::intern::cpp_free(p)

#define STBIW_ASSERT(x)
#define STBIW_MALLOC(size) lib::intern::cpp_malloc(size)
#define STBIW_REALLOC_SIZED(oldbuf, oldsize, newsize) lib::intern::cpp_realloc(oldbuf, oldsize, newsize)
#define STBIW_FREE(p) lib::intern::cpp_free(p)
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
