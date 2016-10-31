#include "rescale.h"

double lerp(double x, double a, double b)
{
    return a + x * (b - a);
}

double de_lerp(double x, double a, double b)
{
    return (x - a) / (b - a);
}
