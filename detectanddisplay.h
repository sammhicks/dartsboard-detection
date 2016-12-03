#pragma once

#include "calcf1.h"

void detectAndDisplay(cv::Mat &input, cv::CascadeClassifier &cascade, std::vector<cv::Rect> &ground, int trueNumber, std::vector<cv::Rect> &prunedFaces);

const double PRUNING_THRESHOLD = 0.75;

void truePositive(std::vector<cv::Rect> &groundTruth, std::vector<cv::Rect> &faces, double threshold, int &total, int &othertrue);

void pruneFaces(cv::Mat &input, std::vector<cv::Rect> &faces, std::vector<cv::Rect> &prunedFaces, double threshold);
