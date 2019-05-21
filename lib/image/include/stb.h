namespace stb
{
#include <cassert>
#include <algorithm>

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


#define STBI_NO_GIF
#define STBI_MALLOC(size) cpp_malloc(size)
#define STBI_REALLOC_SIZED(oldbuf, oldsize, newsize) cpp_realloc(oldbuf, oldsize, newsize)
#define STBI_FREE(p) cpp_free(p)

#define STBIW_MALLOC(size) cpp_malloc(size)
#define STBIW_REALLOC_SIZED(oldbuf, oldsize, newsize) cpp_realloc(oldbuf, oldsize, newsize)
#define STBIW_FREE(p) cpp_free(p)

#define STB_IMAGE_IMPLEMENTATION 
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
}