#include "filter_list.h"

std::vector<cv::Vec4d> filter_list(std::vector<cv::Vec4d> circles, double min_distance)
{
    std::vector<cv::Vec4d> filtered_circles;

    for (cv::Vec4d &circle: circles)
    {
        bool circle_updated = false;

        for (auto circles_it = filtered_circles.begin(); circles_it != filtered_circles.end(); ++circles_it)
        {
            double
                    current_a = (*circles_it)[0],
                    current_b = (*circles_it)[1],
                    current_r = (*circles_it)[2],
                    current_h = (*circles_it)[3],
                    new_a = circle[0],
                    new_b = circle[1],
                    new_r = circle[2],
                    new_h = circle[3],
                    dist = cv::norm(cv::Vec2d(current_a, current_b), cv::Vec2d(new_a, new_b)),
                    r_sum = current_r + new_r;

            if (dist < min_distance * r_sum)
            {
                if (new_h > current_h)
                {
                    (*circles_it) = circle;
                }

                circle_updated = true;

                break;
            }
        }

        if (!circle_updated)
        {
            filtered_circles.push_back(circle);
        }
    }

    return filtered_circles;
}
