#include "hough.h"

void hough(cv::Mat mag, cv::Mat dir, cv::Mat &hough_space, double rmin, double rmax, double mag_threshold, double hough_threshold, std::vector<cv::Vec4d> &circles)
{
    int
            ymin = 0,
            ymax = mag.size[0],
            xmin = 0,
            xmax = mag.size[1],
            r_index_min = 0,
            r_index_max = hough_space.size[0],
            b_index_min = 0,
            b_index_max = hough_space.size[1],
            a_index_min = 0,
            a_index_max = hough_space.size[2];

    double
            bmin = ymin - rmax,
            bmax = ymax + rmax,
            amin = xmin - rmax,
            amax = xmax + rmax;

    for (int y = ymin; y < ymax; ++y)
    {
        for (int x = xmin; x < xmax; ++x)
        {
            for (int r_index = r_index_min; r_index < r_index_max; ++r_index)
            {
                double r = lerp(de_lerp(r_index, r_index_min, r_index_max), rmin, rmax);

                auto mag_val = mag.at<uint8_t>(y, x);

                if (mag_val > mag_threshold)
                {
                    auto dir_val = dir.at<double>(y, x);

                    double
                            b = y + r * sin(dir_val),
                            a = x + r * cos(dir_val);

                    int
                            b_index = static_cast<int>(lerp(de_lerp(b, bmin, bmax), b_index_min, b_index_max)),
                            a_index = static_cast<int>(lerp(de_lerp(a, amin, amax), a_index_min, a_index_max));


                    int &hough_space_val = hough_space.at<int>(r_index, b_index, a_index);

                    ++hough_space_val;
                }
            }
        }
    }

    double
            hough_min = DBL_MAX,
            hough_max = DBL_MIN;

    for (int r_index = r_index_min; r_index < r_index_max; ++r_index)
    {
        for (int b_index = b_index_min; b_index < b_index_max; ++b_index)
        {
            for (int a_index = a_index_min; a_index < a_index_max; ++a_index)
            {
                auto hough_space_val = hough_space.at<int>(r_index, b_index, a_index);

                if (hough_space_val < hough_min)
                {
                    hough_min = hough_space_val;
                }

                if (hough_space_val > hough_max)
                {
                    hough_max = hough_space_val;
                }
            }
        }
    }

    for (int r_index = r_index_min; r_index < r_index_max; ++r_index)
    {
        for (int b_index = b_index_min; b_index < b_index_max; ++b_index)
        {
            for (int a_index = a_index_min; a_index < a_index_max; ++a_index)
            {
                auto hough_space_val = hough_space.at<int>(r_index, b_index, a_index);

                double scaled_hough_space_val = de_lerp(hough_space_val, hough_min, hough_max);

                if (scaled_hough_space_val > hough_threshold)
                {
                    double
                            r = lerp(de_lerp(r_index, r_index_min, r_index_max), rmin, rmax),
                            b = lerp(de_lerp(b_index, b_index_min, b_index_max), bmin, bmax),
                            a = lerp(de_lerp(a_index, a_index_min, a_index_max), amin, amax);

                    circles.push_back(cv::Vec4d(a, b, r, hough_space_val));
                }
            }
        }
    }
}
