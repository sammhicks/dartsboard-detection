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

#include "dynamic_threshold.h"
#include "filter_list.h"
#include "hough.h"
#include "namedimage.h"
#include "sobel.h"

/** Function Headers */
void detectAndDisplay( cv::Mat frame );

/** Global variables */
cv::String cascade_name = "../dartcascade/cascade.xml";
cv::CascadeClassifier cascade;


/** @function main */
int main( int argc, const char** argv )
{
    for (int image_num = 1; image_num < argc; ++image_num)
    {
        cv::Mat source = cv::imread(argv[image_num], CV_LOAD_IMAGE_GRAYSCALE);

        cv::Mat mag, dir;

        sobel(source, mag, dir, 5);

        NamedImage::showImage(NamedImage(mag, "Mag"));

        cv:: Mat thresholded_mag;

        dynamic_threshold(mag, thresholded_mag, 100.0, 255.0, CV_8U);

        NamedImage::showImage(NamedImage(thresholded_mag, "Thresholded Mag"));

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

        NamedImage::showImage(NamedImage(thresholded_mag, "Lines Removed"));

        std::vector<int> hough_space_size = {64, 256, 256};

        cv::Mat hough_space(hough_space_size.size(), &(hough_space_size[0]), CV_32S, cv::Scalar(0));

        auto circles = hough_circle(mag, dir, hough_space, 10, 200, 0.85);

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

        NamedImage::showImage(NamedImage(circles_with_overlay, "Circles"));
    }

    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( cv::Mat frame )
{
    std::vector<cv::Rect> faces;
    cv::Mat frame_gray;

    // 1. Prepare Image by turning it into Grayscale and normalising lighting
    cv::cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    // 2. Perform Viola-Jones Object Detection
    cascade.detectMultiScale( frame_gray, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, cv::Size(50, 50), cv::Size(500,500) );
    // 3. Print number of Faces found
    std::cout << faces.size() << std::endl;

    // 4. Draw box around faces found
    for( unsigned int i = 0; i < faces.size(); i++ )
    {
        cv::rectangle(frame, cv::Point(faces[i].x, faces[i].y), cv::Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), cv::Scalar( 0, 255, 0 ), 2);
    }
}
