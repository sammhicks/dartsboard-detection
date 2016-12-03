#pragma once

#include <vector>

#include <opencv2/opencv.hpp>

const double F1_THRESHOLD = 0.5;

double calcf1(std::vector<cv::Rect> &groundTruth, std::vector<cv::Rect> &faces, int trueNumberOfBoards);
void truePositive(std::vector<cv::Rect> &groundTruth, std::vector<cv::Rect> &faces, int & total, int & othertruedetections);
bool centresThresh(cv::Rect g, cv::Rect f, double thresh);
