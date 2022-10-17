#include <math.h>

#define clamp(X, MIN, MAX) \
    _Generic((X), int : iclamp, float : fclampf, double : fclamp)(X, MIN, MAX)

int    iclamp(int x, int min, int max);
float  fclampf(float x, float min, float max);
double fclamp(double x, double min, double max);

#define min(X, Y) _Generic((X), int : imin, float : fmin, double : fminf)(X, Y)

int imin(int x, int y);

#define max(X, Y) _Generic((X), int : imax, float : fmax, double : fmaxf)(X, Y)

int imax(int x, int y);
