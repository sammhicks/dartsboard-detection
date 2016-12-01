#pragma once

#include <iostream>
#include <limits>
#include <vector>

#include <opencv2/opencv.hpp>

#include "rescale.h"

std::vector<cv::Vec4d> hough(cv::Mat mag, cv::Mat dir, cv::Mat &thresholded_mag, cv::Mat &hough_space, double rmin, double rmax, double mag_threshold, double hough_threshold);
