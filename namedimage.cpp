#include "namedimage.h"

NamedImage::NamedImage(cv::Mat &image, std::__cxx11::string name):
    image(image), name(name)
{
}

void NamedImage::show()
{
    cv::namedWindow(name);
    cv::imshow(name, image);

    cv::waitKey();

    cv::destroyWindow(name);
}
