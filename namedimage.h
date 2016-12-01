#pragma once

#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

class NamedImage
{
private:
    cv::Mat &image;
    const cv::string &name;

public:
    NamedImage(cv::Mat &image, const cv::string &name);

    static void showImage(NamedImage image);
    static void showImages(std::vector<NamedImage> &images);
};
