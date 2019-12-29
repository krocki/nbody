#include <math.h>
#include <stdlib.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

float randnf(float mean, float std) {
    float  x = (float)rand() / RAND_MAX,
           y = (float)rand() / RAND_MAX,
           z = sqrtf(-2 * logf(x)) * cos(2 * M_PI * y);
    return std*z + mean;
}
