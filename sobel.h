#pragma once

#include <opencv2/opencv.hpp>

#include "rescale.h"

void sobel(cv::Mat &image, cv::Mat &mag, cv::Mat &dir, int sobelSize);
