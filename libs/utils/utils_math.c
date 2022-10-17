#include "utils_math.h"

int iclamp(int x, int min, int max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}
float fclampf(float x, float min, float max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}
double fclamp(double x, double min, double max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

int imin(int x, int y) {
    return y < x ? y : x;
}

int imax(int x, int y) {
    return y > x ? y : x;
}
