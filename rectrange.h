#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

void rectRange(std::vector<cv::Rect> rects, cv::Vec2i &min, cv::Vec2i &max);
