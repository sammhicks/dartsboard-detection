#include "namedimage.h"

void NamedImage::startShow() const
{
    cv::namedWindow(name);
    cv::imshow(name, image);
}

void NamedImage::endShow() const
{
    cv::destroyWindow(name);
}

NamedImage::NamedImage(cv::Mat &image, std::__cxx11::string name):
    image(image), name(name)
{
}

void NamedImage::show() const
{
    startShow();

    cv::waitKey();

    endShow();
}

void NamedImage::showMany(std::vector<NamedImage> images)
{
    for (const NamedImage &image: images)
    {
        image.startShow();
    }

    cv::waitKey();

    for (const NamedImage &image: images)
    {
        image.endShow();
    }
}
