#include "namedimage.h"

void NamedImage::startShow(int x, int y) const
{
    cv::namedWindow(name);
    cv::moveWindow(name, x, y);
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
    startShow(0,0);

    cv::waitKey();

    endShow();
}

void NamedImage::showMany(std::vector<NamedImage> images)
{
    int x, y;

    for (int imageNum = 0; imageNum < images.size(); imageNum++)
    {
        y = imageNum / 3 * 500;
        x = imageNum % 3 * 640;
        images[imageNum].startShow(x, y);
    }

    cv::waitKey();

    for (const NamedImage &image: images)
    {
        image.endShow();
    }
}
