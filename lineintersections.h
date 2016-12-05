#pragma once

#include <map>
#include <vector>

#include <opencv2/opencv.hpp>

class LineIntersection
{
private:
    LineIntersection *set;
    unsigned int setSize;

public:
    cv::Vec2f position;

    unsigned int count;

    LineIntersection(cv::Vec2f position);

    LineIntersection(cv::Vec2f a, cv::Vec2f b);

    LineIntersection(const std::vector<const LineIntersection *> &intersections);

    LineIntersection *getSet() const;
    unsigned int getSetSize() const;

    void merge(LineIntersection &other);

    static cv::Vec2f intersection(cv::Vec2f a, cv::Vec2f b);

    static std::vector<LineIntersection> fromLines(const std::vector<cv::Vec2f> &lines);

    static std::vector<LineIntersection> fromLines(const std::vector<cv::Vec2f> &lines, float mergeDistance, unsigned int setSizeThreshold);
};
