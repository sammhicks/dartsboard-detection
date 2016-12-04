/////////////////////////////////////////////////////////////////////////////
//
// COMS30121 - face.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <iostream>

#include "detectanddisplay.h"
#include "dynamicthreshold.h"
#include "filterlist.h"
#include "ground.h"
#include "hough.h"
#include "namedimage.h"
#include "rectrange.h"
#include "sobel.h"

const String CASCADE_NAME = "../dartcascade/cascade.xml";

const double MAG_THRESHOLD = 0.5;

const int HOUGH_LINE_RHO_RESOLUTION = 512;
const int HOUGH_LINE_THETA_RESOLUTION = 512;

const double HOUGH_LINE_THRESHOLD = 0.8;
const double HOUGH_LINE_DIRRANGE = 20.0 * M_PI / 180.0;

const int HOUGH_CIRCLE_R_RESOLUTION = 64;
const int HOUGH_CIRCLE_AB_RESOLUTION = 256;

const double HOUGH_RMIN_RATIO = 0.75;
const double HOUGH_RMAX_RATIO = 1.5;
const double HOUGH_CIRCLE_THRESHOLD = 0.8;

const double FILTER_MINDISTANCE = 1.0;

int main( int argc, const char** argv )
{
    CascadeClassifier cascade;

    if (!cascade.load(CASCADE_NAME)) {
        printf("--(!)Error loading\n"); return EXIT_FAILURE;
    };

    for (int imageNum = 1; imageNum < argc; ++imageNum) {
        int imageID = imageNum - 1;

        cv::Mat input = imread(argv[imageNum], CV_LOAD_IMAGE_COLOR);

        cv::Mat input_gray;
        cv::cvtColor( input, input_gray, CV_BGR2GRAY );

        cv::Mat input_with_overlay = input.clone();

        //std::cout << "-----------dart " << imageID << "--------------" << std::endl;

        std::vector<cv::Rect> prunedFaceDetections = detectAndDisplay(input_gray, cascade, dartsGT[imageID], dartNumbersGT[imageID]);

        for(const cv::Rect &face: prunedFaceDetections)
        {
            cv::rectangle(input_with_overlay, cv::Point(face.x, face.y), cv::Point(face.x + face.width, face.y + face.height), cv::Scalar( 0, 255, 0 ), 2);
        }

        NamedImage(input_with_overlay, "Faces").show();

       // std::stringstream name;
        //name << "pruneFACEStest" << imageID << ".jpg";

        //imwrite(name.str(), input);
    //}

        cv::Mat mag, dir;

        sobel(input_gray, mag, dir, 5);

        NamedImage(mag, "Mag").show();

        cv:: Mat thresholded_mag;

        dynamicThreshold(mag, thresholded_mag, MAG_THRESHOLD, 255.0, CV_8U);

        NamedImage(thresholded_mag, "Thresholded Mag").show();

        // Remove lines

        cv::Mat line_hough_space(HOUGH_LINE_RHO_RESOLUTION, HOUGH_LINE_THETA_RESOLUTION, CV_32S, cv::Scalar(0));

        std::vector<cv::Vec3d> lines = houghLine(thresholded_mag, dir, line_hough_space, HOUGH_LINE_THRESHOLD, HOUGH_LINE_DIRRANGE);

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

        NamedImage(thresholded_mag, "Lines Removed").show();

        std::vector<int> hough_space_size = {HOUGH_CIRCLE_R_RESOLUTION, HOUGH_CIRCLE_AB_RESOLUTION, HOUGH_CIRCLE_AB_RESOLUTION};

        cv::Mat hough_space(hough_space_size.size(), &(hough_space_size[0]), CV_32S, cv::Scalar(0));

        cv::Vec2i minRect, maxRect;

        rectRange(prunedFaceDetections, minRect, maxRect);

        double rMin = HOUGH_RMIN_RATIO * (minRect[0] < minRect[1] ? minRect[0] : minRect[1]) / 2.0;

        double rMax = HOUGH_RMAX_RATIO * (minRect[0] > minRect[1] ? minRect[0] : minRect[1]) / 2.0;

        std::vector<cv::Vec4d> circles = houghCircle(mag, dir, hough_space, rMin, rMax, HOUGH_CIRCLE_THRESHOLD);

        std::vector<cv::Vec4d> filtered_circles = filterList(circles, prunedFaceDetections, FILTER_MINDISTANCE);

        for (auto &circle: circles)
        {
            cv::circle(input_with_overlay, cv::Point(circle[0], circle[1]), circle[2], cvScalar(255, 0, 255));
        }

        for (auto &circle: filtered_circles)
        {
            cv::circle(input_with_overlay, cv::Point(circle[0], circle[1]), circle[2], cvScalar(0, 255, 0));
        }

        NamedImage(input_with_overlay, "Circles").show();

        /*std::stringstream name;
        name << "prunedFACES+HOUGH" << imageID << ".jpg";

        imwrite(name.str(), input);*/
    }

    return 0;
}
