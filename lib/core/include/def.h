#pragma once

#ifdef WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif // WIN32

constexpr double M_PI = 3.14159265358979323846;
