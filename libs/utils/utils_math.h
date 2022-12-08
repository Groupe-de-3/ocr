#pragma once
#include <math.h>

#define clamp(X, MIN, MAX) \
    _Generic((X), int : iclamp, float : fclampf, double : fclamp)(X, MIN, MAX)

inline static  int iclamp(int x, int min, int max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}
inline static float fclampf(float x, float min, float max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}
inline static double fclamp(double x, double min, double max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

#define min(X, Y) _Generic((X), int : imin, float : fmin, double : fminf)(X, Y)

inline static int imin(int x, int y) {
    return y < x ? y : x;
}

inline static int imax(int x, int y) {
    return y > x ? y : x;
}

#define max(X, Y) _Generic((X), int : imax, float : fmax, double : fmaxf)(X, Y)
