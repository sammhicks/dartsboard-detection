#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

#include "inrange.h"

std::vector<cv::Vec4d> filterList(std::vector<cv::Vec4d> circles, std::vector<cv::Rect> faces, double min_distance);
