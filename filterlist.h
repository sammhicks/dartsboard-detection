#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

std::vector<cv::Vec4d> filterList(std::vector<cv::Vec4d> circles, double min_distance);
