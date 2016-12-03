#pragma once

#include <opencv2/opencv.hpp>

#include "rescale.h"

double dynamicThreshold(cv::InputArray src, cv::OutputArray dst, double thresh, double maxval, int type);
