#pragma once

#include <iostream>
#include <limits>
#include <vector>

#include <opencv2/opencv.hpp>

#include "namedimage.h"
#include "rescale.h"

inline bool in_range(int x, int a, int b);

std::vector<cv::Vec3d> hough_line(cv::Mat mag, cv::Mat dir, cv::Mat &hough_space, double hough_threshold, double dir_range);

std::vector<cv::Vec4d> hough_circle(cv::Mat mag, cv::Mat dir, cv::Mat &hough_space, double rmin, double rmax, double hough_threshold);
