#pragma once

#include <vector>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

extern std::vector< vector< Rect>> dartsGT;
extern std::vector< int> dartNumbersGT;
