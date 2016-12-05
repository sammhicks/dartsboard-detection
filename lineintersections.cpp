#include "lineintersections.h"


LineIntersection::LineIntersection(cv::Vec2f position):
    set(NULL),
    setSize(1),
    position(position),
    count(0)
{
}

LineIntersection::LineIntersection(cv::Vec2f a, cv::Vec2f b):
    LineIntersection(intersection(a, b))
{
}

LineIntersection::LineIntersection(const std::vector<const LineIntersection*> &intersections):
    set(NULL),
    setSize(1)
{
    position = cv::Vec2f(0.0, 0.0);

    for (const LineIntersection *intersection: intersections)
    {
        position += intersection->position;
    }

    count = intersections.size();

    position *= (1.0 / count);
}

LineIntersection *LineIntersection::getSet() const
{
    return set == NULL ? const_cast<LineIntersection *>(this) : set->getSet();
}

unsigned int LineIntersection::getSetSize() const
{
    return getSet()->setSize;
}

void LineIntersection::merge(LineIntersection &other)
{
    LineIntersection
            *mySet = getSet(),
            *otherSet = other.getSet();

    if (mySet != otherSet)
    {
        unsigned int
                mySize = getSetSize(),
                otherSize = other.getSetSize();

        unsigned int sizeSum = mySize + otherSize;

        if (getSetSize() < other.getSetSize())
        {
            mySet->set = otherSet;
            otherSet->setSize = sizeSum;
        }
        else
        {
            otherSet->set = mySet;
            mySet->setSize = sizeSum;
        }
    }
}

cv::Vec2f LineIntersection::intersection(cv::Vec2f a, cv::Vec2f b)
{
    float
            rhoA = a[0],
            thetaA = a[1],
            rhoB = b[0],
            thetaB = b[1];

    float
            sA = sin(thetaA),
            cA = cos(thetaA),
            sB = sin(thetaB),
            cB = cos(thetaB),
            csc = 1.0 / sin(thetaA - thetaB);

    cv::Mat result = csc * (cv::Mat_<float>(2, 2) << -sB, sA, cB, -cA) * (cv::Mat_<float>(2, 1) << rhoA, rhoB);

    float
            x = result.at<float>(0, 0),
            y = result.at<float>(1, 0);

    return cv::Vec2f(x, y);
}

std::vector<LineIntersection> LineIntersection::fromLines(const std::vector<cv::Vec2f> &lines)
{
    std::vector<LineIntersection> intersections;

    for (auto firstLineIt = lines.begin(); firstLineIt != lines.end(); ++firstLineIt)
    {
        for (auto secondLineIt = (firstLineIt + 1); secondLineIt != lines.end(); ++secondLineIt)
        {
            intersections.emplace_back(*firstLineIt, *secondLineIt);
        }
    }

    return intersections;
}

std::vector<LineIntersection> LineIntersection::fromLines(const std::vector<cv::Vec2f> &lines, float mergeDistance, unsigned int setSizeThreshold)
{
    std::vector<LineIntersection> intersections = fromLines(lines);

    for (auto firstIntersectionIt = intersections.begin(); firstIntersectionIt != intersections.end(); ++firstIntersectionIt)
    {
        for (auto secondIntersectionIt = (firstIntersectionIt + 1); secondIntersectionIt != intersections.end(); ++secondIntersectionIt)
        {
            if (cv::norm(firstIntersectionIt->position - secondIntersectionIt->position) < mergeDistance)
            {
                firstIntersectionIt->merge(*secondIntersectionIt);
            }
        }
    }

    std::map<LineIntersection*, std::vector<const LineIntersection*>> sets;

    for (const LineIntersection &intersection: intersections)
    {
        auto set = sets.find(intersection.getSet());

        if (set == sets.end())
        {
            sets[intersection.getSet()] = {&intersection};
        }
        else
        {
            sets[intersection.getSet()].push_back(&intersection);
        }
    }

    std::vector<LineIntersection> filteredIntersections;

    for (const auto& kv : sets)
    {
        if (kv.second.size() >= setSizeThreshold)
        {
            filteredIntersections.emplace_back(kv.second);
        }
    }

    return filteredIntersections;
}
