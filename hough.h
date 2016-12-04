#pragma once

#include <iostream>
#include <limits>
#include <vector>

#include <opencv2/opencv.hpp>

#include "inrange.h"
#include "namedimage.h"
#include "rescale.h"

std::vector<cv::Vec3d> houghLine(cv::Mat mag, cv::Mat dir, cv::Mat &houghSpace, double houghThreshold, double dirRange);

std::vector<cv::Vec4d> houghCircle(cv::Mat mag, cv::Mat dir, cv::Mat &houghSpace, double rmin, double rmax, double houghThreshold);
