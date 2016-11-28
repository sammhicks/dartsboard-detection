#pragma once

#include <iostream>
#include <limits>
#include <vector>

#include "opencv/cv.h"

#include "rescale.h"

std::vector<cv::Vec4d> hough(cv::Mat mag, cv::Mat dir, cv::Mat &hough_space, double rmin, double rmax, double mag_threshold, double hough_threshold);
