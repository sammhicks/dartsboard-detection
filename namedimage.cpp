#include "namedimage.h"

NamedImage::NamedImage(cv::Mat &image, std::__cxx11::string name):
    image(image), name(name)
{
}

void NamedImage::showImage(NamedImage image)
{
    std::vector<NamedImage> images = {image};

    showImages(images);
}

void NamedImage::showImages(std::vector<NamedImage> &images)
{
    for (NamedImage &image: images)
    {
        cv::namedWindow(image.name);
        cv::imshow(image.name, image.image);
    }

    cv::waitKey();

    for (NamedImage &image: images)
    {
        cv::destroyWindow(image.name);
    }
}
