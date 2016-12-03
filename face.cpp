/////////////////////////////////////////////////////////////////////////////
//
// COMS30121 - face.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <iostream>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "detectanddisplay.h"
#include "dynamicthreshold.h"
#include "filterlist.h"
#include "hough.h"
#include "namedimage.h"
#include "sobel.h"
#include <string>
#include "ground.h"

const String CASCADE_NAME = "../dartcascade/cascade.xml";

/** @function main */
int main( int argc, const char** argv )
{
    CascadeClassifier cascade;

    if (!cascade.load(CASCADE_NAME)) {
        printf("--(!)Error loading\n"); return EXIT_FAILURE;
    };

    for (int imageNum = 1; imageNum < argc; ++imageNum) {
        int imageID = imageNum - 1;

        cv::Mat input = imread(argv[imageNum], CV_LOAD_IMAGE_COLOR);

        //std::cout << "-----------dart " << imageID << "--------------" << std::endl;

        std::vector<cv::Rect> prunedFaceDetections;

        detectAndDisplay(input, cascade, dartsGT[imageID], dartNumbersGT[imageID], prunedFaceDetections);

        std::stringstream name;
        name << "pruneFACEStest" << imageID << ".jpg";

        imwrite(name.str(), input);
    }


    //////HOUGH SPACE TESTING LOOP...
   /* for
    {
        cv::Mat source = cv::imread(argv[image_num], CV_LOAD_IMAGE_GRAYSCALE);

        ss << image_num-1;
        name = "circles10-200-0.8"+ss.str()+".jpg";

        cv::Mat mag, dir;

        sobel(source, mag, dir, 5);

        //NamedImage::showImage(NamedImage(mag, "Mag"));

        cv:: Mat thresholded_mag;

        dynamic_threshold(mag, thresholded_mag, 100.0, 255.0, CV_8U);

        //NamedImage::showImage(NamedImage(thresholded_mag, "Thresholded Mag"));

        // Remove lines

        cv::Mat line_hough_space(512, 512, CV_32S, cv::Scalar(0));

        std::vector<cv::Vec3d> lines = hough_line(thresholded_mag, dir, line_hough_space, 0.8, 20.0 * M_PI / 180.0);

        for(cv::Vec3d &line: lines)
        {
            float
                    rho   = line[0],
                    theta = line[1];

            double
                    c = cos(theta),
                    s = sin(theta);

            cv::Point ref(cvRound(c * rho), cvRound(s * rho));

            double line_mag = 1000.0;

            cv::Point offset(cvRound(line_mag * -s), cvRound(line_mag * c));

            cv::line(thresholded_mag, ref - offset, ref + offset, cv::Scalar(0l), 7, CV_AA);
        }

        //NamedImage::showImage(NamedImage(thresholded_mag, "Lines Removed"));

        std::vector<int> hough_space_size = {64, 256, 256};

        cv::Mat hough_space(hough_space_size.size(), &(hough_space_size[0]), CV_32S, cv::Scalar(0));

        auto circles = hough_circle(mag, dir, hough_space, 10, 200, 0.8);

        auto filtered_circles = filter_list(circles, 1.0);

        cv::Mat circles_with_overlay(source.size(), CV_8UC3);

        cv::cvtColor(source, circles_with_overlay, cv::COLOR_GRAY2BGR);

        for (auto &circle: circles)
        {
            cv::circle(circles_with_overlay, cv::Point(circle[0], circle[1]), circle[2], cvScalar(255, 0, 255));
        }

        for (auto &circle: filtered_circles)
        {
            cv::circle(circles_with_overlay, cv::Point(circle[0], circle[1]), circle[2], cvScalar(0, 255, 0));
        }

        //NamedImage::showImage(NamedImage(circles_with_overlay, "Circles"));

        imwrite(name, circles_with_overlay);
    } */

    return 0;
}
