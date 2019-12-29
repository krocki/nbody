#include <math.h>
#include <stdlib.h>
float randnf(float mean, float std) {
    float  x = (float)rand() / RAND_MAX,
           y = (float)rand() / RAND_MAX,
           z = sqrtf(-2 * logf(x)) * cos(2 * M_PI * y);
    return std*z + mean;
}
