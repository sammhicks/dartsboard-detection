#include "filterlist.h"

std::vector<cv::Vec4d> filterList(std::vector<cv::Vec4d> circles, std::vector<cv::Rect> faces, double min_distance)
{
    std::vector<cv::Vec4d> filtered_circles;

    for (cv::Vec4d &circle: circles)
    {
        double
                new_a = circle[0],
                new_b = circle[1],
                new_r = circle[2],
                new_h = circle[3];

        bool insideRectangle = false;

        for (const cv::Rect &rect: faces)
        {
            if (inRange(new_a, rect.x, rect.x + rect.width) && inRange(new_b, rect.y, rect.y + rect.height))
            {
                insideRectangle = true;

                break;
            }
        }

        if (insideRectangle)
        {
            bool circleUpdated = false;

            for (auto circlesIt = filtered_circles.begin(); circlesIt != filtered_circles.end(); ++circlesIt)
            {
                double
                        current_a = (*circlesIt)[0],
                        current_b = (*circlesIt)[1],
                        current_r = (*circlesIt)[2],
                        current_h = (*circlesIt)[3],
                        dist = cv::norm(cv::Vec2d(current_a, current_b), cv::Vec2d(new_a, new_b)),
                        r_sum = current_r + new_r;

                if (dist < min_distance * r_sum)
                {
                    if (new_h > current_h)
                    {
                        (*circlesIt) = circle;
                    }

                    circleUpdated = true;

                    break;
                }
            }

            if (!circleUpdated)
            {
                filtered_circles.push_back(circle);
            }
        }
    }

    return filtered_circles;
}
