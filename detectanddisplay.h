#pragma once

#include "calcf1.h"

const double PRUNING_THRESHOLD = 0.40;

std::vector<cv::Rect> detectAndDisplay(cv::Mat &input, cv::CascadeClassifier &cascade, std::vector<cv::Rect> &ground, int trueNumber);

void truePositive(std::vector<cv::Rect> &groundTruth, std::vector<cv::Rect> &faces, double threshold, int &total, int &othertrue);

std::vector<cv::Rect> pruneFaces(cv::Mat &input, std::vector<cv::Rect> &faces);
