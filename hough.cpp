#include "hough.h"

bool in_range(int x, int a, int b)
{
    return x >= a && x < b;
}

std::vector<cv::Vec3d> hough_line(cv::Mat mag, cv::Mat dir, cv::Mat &hough_space, double hough_threshold, double dir_range)
{
    int
            ymin = 0,
            ymax = mag.size[0],
            xmin = 0,
            xmax = mag.size[1],
            rho_index_min = 0,
            rho_index_max = hough_space.size[0],
            theta_index_min = 0,
            theta_index_max = hough_space.size[1];

    double diagonal_distance = cv::norm(cv::Point2f(xmax, ymax));

    double
            rho_min = -diagonal_distance,
            rho_max =  diagonal_distance,
            theta_min = 0.0,
            theta_max = 2 * M_PI;

    for (int y = ymin; y < ymax; ++y)
    {
        for (int x = xmin; x < xmax; ++x)
        {
            uint8_t mag_val = mag.at<uint8_t>(y, x);
            double  dir_val =  dir.at<double>(y,x);

            if (mag_val > 127)
            {
                int
                        theta_index_range_min = lerp(de_lerp(dir_val - dir_range, theta_min, theta_max), theta_index_min, theta_index_max),
                        theta_index_range_max = lerp(de_lerp(dir_val + dir_range, theta_min, theta_max), theta_index_min, theta_index_max);

                if (theta_index_range_min < theta_index_min)
                {
                    theta_index_range_min = theta_index_min;
                }

                if (theta_index_range_max < theta_index_max)
                {
                    theta_index_range_max = theta_index_max;
                }

                for (int theta_index = theta_index_range_min; theta_index < theta_index_range_max; ++theta_index)
                {
                    double theta = lerp(de_lerp(theta_index, theta_index_min, theta_index_max), theta_min, theta_max);

                    double rho = x * cos(theta) + y * sin(theta);

                    int rho_index = lerp(de_lerp(rho, rho_min, rho_max), rho_index_min, rho_index_max);

                    if (in_range(rho_index, rho_index_min, rho_index_max))
                    {
                        int &hough_space_val = hough_space.at<int>(rho_index, theta_index);

                        ++hough_space_val;
                    }
                }
            }
        }
    }

    double
            hough_min = DBL_MAX,
            hough_max = DBL_MIN;

    for (int rho_index = rho_index_min; rho_index < rho_index_max; ++rho_index)
    {
        for (int theta_index = theta_index_min; theta_index < theta_index_max; ++theta_index)
        {
            int &hough_space_val = hough_space.at<int>(rho_index, theta_index);

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

    cv::Mat hough_image(hough_space.size(), CV_8U);

    for (int rho_index = rho_index_min; rho_index < rho_index_max; ++rho_index)
    {
        for (int theta_index = theta_index_min; theta_index < theta_index_max; ++theta_index)
        {
            int &hough_space_val = hough_space.at<int>(rho_index, theta_index);
            uint8_t &hough_image_val = hough_image.at<uint8_t>(rho_index, theta_index);

            hough_image_val = lerp(de_lerp(hough_space_val, hough_min, hough_max), 0, 255);
        }
    }

    NamedImage::showImage(NamedImage(hough_image, "Hough Space"));

    std::vector<cv::Vec3d> lines;

    for (int rho_index = rho_index_min; rho_index < rho_index_max; ++rho_index)
    {
        for (int theta_index = theta_index_min; theta_index < theta_index_max; ++theta_index)
        {
            int &hough_space_val = hough_space.at<int>(rho_index, theta_index);

            double scaled_hough_space_val = de_lerp(hough_space_val, hough_min, hough_max);

            if (scaled_hough_space_val > hough_threshold)
            {
                double
                        rho = lerp(de_lerp(rho_index, rho_index_min, rho_index_max), rho_min, rho_max),
                        theta = lerp(de_lerp(theta_index, theta_index_min, theta_index_max), theta_min, theta_max);

                cv::Vec3d line(rho, theta, hough_space_val);

                lines.push_back(line);
            }
        }
    }

    return lines;
}

std::vector<cv::Vec4d> hough_circle(cv::Mat mag, cv::Mat dir, cv::Mat &hough_space, double rmin, double rmax, double hough_threshold)
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
            uint8_t mag_val = mag.at<uint8_t>(y, x);

            if (mag_val > 127)
            {
                for (int r_index = r_index_min; r_index < r_index_max; ++r_index)
                {
                    double r = lerp(de_lerp(r_index, r_index_min, r_index_max), rmin, rmax);

                    auto dir_val = dir.at<double>(y, x);

                    double
                            b = y + r * sin(dir_val),
                            a = x + r * cos(dir_val);

                    int
                            b_index = static_cast<int>(lerp(de_lerp(b, bmin, bmax), b_index_min, b_index_max)),
                            a_index = static_cast<int>(lerp(de_lerp(a, amin, amax), a_index_min, a_index_max));

                    if (
                            in_range(b_index, b_index_min, b_index_max) &&
                            in_range(a_index, a_index_min, a_index_max))
                    {
                        int &hough_space_val = hough_space.at<int>(r_index, b_index, a_index);

                        ++hough_space_val;
                    }

                    b = y - r * sin(dir_val);
                    a = x - r * cos(dir_val);

                    b_index = static_cast<int>(lerp(de_lerp(b, bmin, bmax), b_index_min, b_index_max));
                    a_index = static_cast<int>(lerp(de_lerp(a, amin, amax), a_index_min, a_index_max));

                    if (
                            in_range(b_index, b_index_min, b_index_max) &&
                            in_range(a_index, a_index_min, a_index_max))
                    {
                        int &hough_space_val = hough_space.at<int>(r_index, b_index, a_index);

                        ++hough_space_val;
                    }
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

    std::vector<cv::Vec4d> circles;

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

    return circles;
}
