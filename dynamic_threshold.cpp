#include "dynamic_threshold.h"

double dynamic_threshold(cv::InputArray src, cv::OutputArray dst, double thresh, double maxval, int type)
{
    double input_min, input_max;

    cv::minMaxLoc(src, &input_min, &input_max);

    return cv::threshold(src, dst, maxval * de_lerp(thresh, input_min, input_max), maxval, type);
}
