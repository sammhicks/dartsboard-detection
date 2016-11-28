#pragma once

#include "opencv/cv.h"

void sobel(cv::Mat &image, cv::Mat &mag, cv::Mat &dir, int sobel_size);
