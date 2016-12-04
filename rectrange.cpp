#include "rectrange.h"

void updateIfSmaller(int &x, int n)
{
    if (n < x)
    {
        x = n;
    }
}

void updateIfLarger(int &x, int n)
{
    if (n > x)
    {
        x = n;
    }
}

void rectRange(std::vector<cv::Rect> rects, cv::Vec2i &min, cv::Vec2i &max)
{
    min[0] = std::numeric_limits<int>::max();
    min[1] = std::numeric_limits<int>::max();

    max[0] = 0;
    max[1] = 0;

    for (const cv::Rect &rect: rects)
    {
        updateIfSmaller(min[0], rect.width);
        updateIfSmaller(min[1], rect.height);

        updateIfLarger(max[0], rect.width);
        updateIfLarger(max[1], rect.height);
    }
}
