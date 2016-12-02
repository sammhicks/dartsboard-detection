#include "sobel.h"

void sobel(cv::Mat &image, cv::Mat &mag, cv::Mat &dir, int sobel_size)
{
    cv::Mat dx, dy;

    cv::Sobel(image, dx, CV_64F, 1, 0, sobel_size);
    cv::Sobel(image, dy, CV_64F, 0, 1, sobel_size);

    cv::Mat unscaled_mag(image.size(), CV_64F);
    dir = cv::Mat(image.size(), CV_64F);

    for (int row = 0; row < image.rows; ++row)
    {
        for (int col = 0; col < image.cols; ++col)
        {
            auto dx_val = dx.at<double>(row, col);
            auto dy_val = dy.at<double>(row, col);
            auto &mag_val = unscaled_mag.at<double>(row, col);
            auto &dir_val = dir.at<double>(row, col);

            mag_val = sqrt(dx_val * dx_val + dy_val * dy_val);
            dir_val = atan2(dy_val, dx_val);
        }
    }

    double mag_min, mag_max;

    cv::minMaxLoc(unscaled_mag, &mag_min, &mag_max);

    mag = cv::Mat(unscaled_mag.size(), CV_8U);

    for (int row = 0; row < image.rows; ++row)
    {
        for (int col = 0; col < image.cols; ++col)
        {
            auto mag_val = unscaled_mag.at<double>(row, col);
            auto &scaled_mag_val = mag.at<uint8_t>(row, col);

            scaled_mag_val = static_cast<uint8_t>(lerp(de_lerp(mag_val, mag_min, mag_max), 0, std::numeric_limits<uint8_t>::max()));
        }
    }
}
