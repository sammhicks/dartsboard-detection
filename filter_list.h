#pragma once

#include <vector>

#include <opencv/cv.h>

std::vector<cv::Vec4d> filter_list(std::vector<cv::Vec4d> circles, double min_distance);
