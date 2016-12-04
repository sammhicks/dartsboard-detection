#include "hough.h"

std::vector<cv::Vec3d> houghLine(cv::Mat mag, cv::Mat dir, cv::Mat &houghSpace, double houghThreshold, double dirRange)
{
    int
            yMin = 0,
            yMax = mag.size[0],
            xMin = 0,
            xMax = mag.size[1],
            rhoIndexMin = 0,
            rhoIndexMax = houghSpace.size[0],
            thetaIndexMin = 0,
            thetaIndexMax = houghSpace.size[1];

    double diagonalDistance = cv::norm(cv::Point2f(xMax, yMax));

    double
            rhoMin = -diagonalDistance,
            rhoMax =  diagonalDistance,
            thetaMin = 0.0,
            thetaMax = 2 * M_PI;

    for (int y = yMin; y < yMax; ++y)
    {
        for (int x = xMin; x < xMax; ++x)
        {
            uint8_t magVal = mag.at<uint8_t>(y, x);
            double  dirVal =  dir.at<double>(y,x);

            if (magVal > 127)
            {
                int
                        thetaIndexRangeMin = lerp(deLerp(dirVal - dirRange, thetaMin, thetaMax), thetaIndexMin, thetaIndexMax),
                        thetaIndexRangeMax = lerp(deLerp(dirVal + dirRange, thetaMin, thetaMax), thetaIndexMin, thetaIndexMax);

                if (thetaIndexRangeMin < thetaIndexMin)
                {
                    thetaIndexRangeMin = thetaIndexMin;
                }

                if (thetaIndexRangeMax < thetaIndexMax)
                {
                    thetaIndexRangeMax = thetaIndexMax;
                }

                for (int thetaIndex = thetaIndexRangeMin; thetaIndex < thetaIndexRangeMax; ++thetaIndex)
                {
                    double theta = lerp(deLerp(thetaIndex, thetaIndexMin, thetaIndexMax), thetaMin, thetaMax);

                    double rho = x * cos(theta) + y * sin(theta);

                    int rho_index = lerp(deLerp(rho, rhoMin, rhoMax), rhoIndexMin, rhoIndexMax);

                    if (inRange(rho_index, rhoIndexMin, rhoIndexMax))
                    {
                        int &hough_space_val = houghSpace.at<int>(rho_index, thetaIndex);

                        ++hough_space_val;
                    }
                }
            }
        }
    }

    double
            houghMin = DBL_MAX,
            houghMax = DBL_MIN;

    for (int rhoIndex = rhoIndexMin; rhoIndex < rhoIndexMax; ++rhoIndex)
    {
        for (int thetaIndex = thetaIndexMin; thetaIndex < thetaIndexMax; ++thetaIndex)
        {
            int &houghSpaceVal = houghSpace.at<int>(rhoIndex, thetaIndex);

            if (houghSpaceVal < houghMin)
            {
                houghMin = houghSpaceVal;
            }

            if (houghSpaceVal > houghMax)
            {
                houghMax = houghSpaceVal;
            }
        }
    }

    cv::Mat houghImage(houghSpace.size(), CV_8U);

    for (int rhoIndex = rhoIndexMin; rhoIndex < rhoIndexMax; ++rhoIndex)
    {
        for (int thetaIndex = thetaIndexMin; thetaIndex < thetaIndexMax; ++thetaIndex)
        {
            int &houghSpaceVal = houghSpace.at<int>(rhoIndex, thetaIndex);
            uint8_t &houghImageVal = houghImage.at<uint8_t>(rhoIndex, thetaIndex);

            houghImageVal = lerp(deLerp(houghSpaceVal, houghMin, houghMax), 0, 255);
        }
    }

    //NamedImage::showImage(NamedImage(hough_image, "Hough Space"));

    std::vector<cv::Vec3d> lines;

    for (int rhoIndex = rhoIndexMin; rhoIndex < rhoIndexMax; ++rhoIndex)
    {
        for (int thetaIndex = thetaIndexMin; thetaIndex < thetaIndexMax; ++thetaIndex)
        {
            int &houghSpaceVal = houghSpace.at<int>(rhoIndex, thetaIndex);

            double scaledHoughSpaceVal = deLerp(houghSpaceVal, houghMin, houghMax);

            if (scaledHoughSpaceVal > houghThreshold)
            {
                double
                        rho = lerp(deLerp(rhoIndex, rhoIndexMin, rhoIndexMax), rhoMin, rhoMax),
                        theta = lerp(deLerp(thetaIndex, thetaIndexMin, thetaIndexMax), thetaMin, thetaMax);

                cv::Vec3d line(rho, theta, houghSpaceVal);

                lines.push_back(line);
            }
        }
    }

    return lines;
}

std::vector<cv::Vec4d> houghCircle(cv::Mat mag, cv::Mat dir, cv::Mat &houghSpace, double rmin, double rmax, double houghThreshold)
{
    int
            yMin = 0,
            yMax = mag.size[0],
            xMin = 0,
            xMax = mag.size[1],
            rIndexMin = 0,
            rIndexMax = houghSpace.size[0],
            bIndexMin = 0,
            bIndexMax = houghSpace.size[1],
            aIndexMin = 0,
            aIndexMax = houghSpace.size[2];

    double
            bMin = yMin - rmax,
            bMax = yMax + rmax,
            aMin = xMin - rmax,
            aMax = xMax + rmax;

    for (int y = yMin; y < yMax; ++y)
    {
        for (int x = xMin; x < xMax; ++x)
        {
            uint8_t magVal = mag.at<uint8_t>(y, x);

            if (magVal > 127)
            {
                for (int rIndex = rIndexMin; rIndex < rIndexMax; ++rIndex)
                {
                    double r = lerp(deLerp(rIndex, rIndexMin, rIndexMax), rmin, rmax);

                    auto dirVal = dir.at<double>(y, x);

                    double
                            b = y + r * sin(dirVal),
                            a = x + r * cos(dirVal);

                    int
                            bIndex = static_cast<int>(lerp(deLerp(b, bMin, bMax), bIndexMin, bIndexMax)),
                            aIndex = static_cast<int>(lerp(deLerp(a, aMin, aMax), aIndexMin, aIndexMax));

                    if (
                            inRange(bIndex, bIndexMin, bIndexMax) &&
                            inRange(aIndex, aIndexMin, aIndexMax))
                    {
                        int &houghSpaceVal = houghSpace.at<int>(rIndex, bIndex, aIndex);

                        ++houghSpaceVal;
                    }

                    b = y - r * sin(dirVal);
                    a = x - r * cos(dirVal);

                    bIndex = static_cast<int>(lerp(deLerp(b, bMin, bMax), bIndexMin, bIndexMax));
                    aIndex = static_cast<int>(lerp(deLerp(a, aMin, aMax), aIndexMin, aIndexMax));

                    if (
                            inRange(bIndex, bIndexMin, bIndexMax) &&
                            inRange(aIndex, aIndexMin, aIndexMax))
                    {
                        int &houghSpaceVal = houghSpace.at<int>(rIndex, bIndex, aIndex);

                        ++houghSpaceVal;
                    }
                }
            }
        }
    }

    double
            houghMin = DBL_MAX,
            houghMax = DBL_MIN;

    for (int r_index = rIndexMin; r_index < rIndexMax; ++r_index)
    {
        for (int b_index = bIndexMin; b_index < bIndexMax; ++b_index)
        {
            for (int a_index = aIndexMin; a_index < aIndexMax; ++a_index)
            {
                auto hough_space_val = houghSpace.at<int>(r_index, b_index, a_index);

                if (hough_space_val < houghMin)
                {
                    houghMin = hough_space_val;
                }

                if (hough_space_val > houghMax)
                {
                    houghMax = hough_space_val;
                }
            }
        }
    }

    std::vector<cv::Vec4d> circles;

    for (int rIndex = rIndexMin; rIndex < rIndexMax; ++rIndex)
    {
        for (int bIndex = bIndexMin; bIndex < bIndexMax; ++bIndex)
        {
            for (int aIndex = aIndexMin; aIndex < aIndexMax; ++aIndex)
            {
                auto houghSpaceVal = houghSpace.at<int>(rIndex, bIndex, aIndex);

                double scaledHoughSpaceVal = deLerp(houghSpaceVal, houghMin, houghMax);

                if (scaledHoughSpaceVal > houghThreshold)
                {
                    double
                            r = lerp(deLerp(rIndex, rIndexMin, rIndexMax), rmin, rmax),
                            b = lerp(deLerp(bIndex, bIndexMin, bIndexMax), bMin, bMax),
                            a = lerp(deLerp(aIndex, aIndexMin, aIndexMax), aMin, aMax);

                    circles.push_back(cv::Vec4d(a, b, r, houghSpaceVal));
                }
            }
        }
    }

    return circles;
}
