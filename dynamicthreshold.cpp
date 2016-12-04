#include "dynamicthreshold.h"

double dynamicThreshold(cv::InputArray src, cv::OutputArray dst, double thresh, double maxVal, int type)
{
    double inputMin, inputMax;

    cv::minMaxLoc(src, &inputMin, &inputMax);

    return cv::threshold(src, dst, lerp(thresh, inputMin, inputMax), maxVal, type);
}
