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
    cv::Vec2d position;

    unsigned int count;

    LineIntersection(cv::Vec2d position);

    LineIntersection(cv::Vec2d a, cv::Vec2d b);

    LineIntersection(const std::vector<const LineIntersection *> &intersections);

    LineIntersection *getSet() const;
    unsigned int getSetSize() const;

    void merge(LineIntersection &other);

    static cv::Vec2d intersection(cv::Vec2d a, cv::Vec2d b);

    static std::vector<LineIntersection> fromLines(const std::vector<cv::Vec2d> &lines);

    static std::vector<LineIntersection> fromLines(const std::vector<cv::Vec2d> &lines, double mergeDistance, unsigned int setSizeThreshold);
};
