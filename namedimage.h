#pragma once

#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

class NamedImage
{
private:
    cv::Mat &image;
    std::string name;

    void startShow(int x, int y) const;
    void endShow() const;

public:
    NamedImage(cv::Mat &image, std::string name);

    void show() const;

    static void showMany(std::vector<NamedImage> images);
};
