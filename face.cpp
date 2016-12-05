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
#include "lineintersections.h"
#include "namedimage.h"
#include "rectrange.h"
#include "sobel.h"

const String CASCADE_NAME = "../dartcascade/cascade.xml";

const double MAG_THRESHOLD = 0.5;

const int HOUGH_LINE_RHO_RESOLUTION = 512;
const int HOUGH_LINE_THETA_RESOLUTION = 512;

const double HOUGH_LINE_THRESHOLD = 0.7;
const double HOUGH_LINE_DIRRANGE = 20.0 * M_PI / 180.0;

const int HOUGH_CIRCLE_R_RESOLUTION = 64;
const int HOUGH_CIRCLE_AB_RESOLUTION = 256;

const double HOUGH_RMIN_RATIO = 0.75;
const double HOUGH_RMAX_RATIO = 2.0;
const double HOUGH_CIRCLE_THRESHOLD = 0.8;

const double FILTER_MINDISTANCE = 1.0;

bool containedInRect(cv::Rect rect, int x, int y);
bool isAroundCircle(cv::Rect rect, std::vector<cv::Vec3f> & opencvcircles);
double euclideanDist(Point p, Point q);
int closestRect(int x, int y, std::vector<cv::Rect> & detections );

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
        cv::cvtColor(input, input_gray, CV_BGR2GRAY);

        cv::Mat input_with_overlay = input.clone();

        //std::cout << "-----------dart " << imageID << "--------------" << std::endl;

        std::vector<cv::Rect> prunedFaceDetections = detectAndDisplay(input_gray, cascade, dartsGT[imageID], dartNumbersGT[imageID]);

        for(const cv::Rect &face: prunedFaceDetections)
        {
            cv::rectangle(input_with_overlay, face, cv::Scalar( 0, 255, 0 ), 2);
        }

    //MASK out all section that are not within a 'face' detection to black
        cv::Mat face_mask_source = imread(argv[imageNum], CV_LOAD_IMAGE_GRAYSCALE);
        //cvtColor(input, face_mask_source, CV_BGR2GRAY);
            cv::Mat face_mask;
            face_mask = face_mask_source.clone();
            face_mask.setTo( cv::Scalar(0));

            for (Rect detection : prunedFaceDetections)
            {
                cv::Mat inputRect = face_mask_source(detection);
                cv::Mat maskRect = face_mask(detection);
                inputRect.copyTo(maskRect);
            }
    //END OF MASK SECTION


    //THIS SECTION DOES THE LINE TRANSFORM ON THE MASKED IMAGE WITH THE OPENCV HOUGH TRANSFORM!
        /*vector<Vec2f> opencvlines;
        cvtColor(face_mask, face_mask, CV_BGR2GRAY);
        blur( faceRect, faceRect, Size(3,3) );
        Canny(faceRect, faceRect, 50, 200, 3);
        HoughLines(faceRect, opencvlines, 1, CV_PI/180, 100, 0, 0 );
        //cvtColor(face_mask,face_mask, CV_GRAY2BGR);
        for( size_t i = 0; i < opencvlines.size(); i++ )
        {
          float rho0 = opencvlines[i][0], theta0 = opencvlines[i][1];
          Point pt1, pt2;
          double a = cos(theta0), b = sin(theta0);
          double x0 = a*rho0, y0 = b*rho0;
          pt1.x = cvRound(x0 + 1000*(-b));
          pt1.y = cvRound(y0 + 1000*(a));
          pt2.x = cvRound(x0 - 1000*(-b));
          pt2.y = cvRound(y0 - 1000*(a));
          line( faceRect, pt1, pt2, Scalar(0,255,0), 1, CV_AA);
        }*/
    //END OF LINE SECTION

    //THIS SECTION PEFORMS THE LINE TRANSFORM ON THE MASKED IMAGE USING SAM'S HOUGH LINE TRANSFORM
        //cvtColor(face_mask,face_mask, CV_GRAY2BGR);

        /*cv::Mat mag0, dir0;

        sobel(face_mask, mag0, dir0, 3);

        cv::Mat line_hough_space0(HOUGH_LINE_RHO_RESOLUTION, HOUGH_LINE_THETA_RESOLUTION, CV_32S, cv::Scalar(0));

        std::vector<cv::Vec3d> opencvlines = houghLine(mag0, dir0, line_hough_space0, 0.8, 20.0 * M_PI / 180.0);

        for (const cv::Vec3d &line: opencvlines)
        {
            float
                    rho0   = line[0],
                    theta0 = line[1];

            double
                    c0 = cos(theta0),
                    s0 = sin(theta0);

            cv::Point ref0(cvRound(c0 * rho0), cvRound(s0 * rho0));

            double line_mag0 = 1000.0;

            cv::Point offset0(cvRound(line_mag0 * -s0), cvRound(line_mag0 * c0));

            cv::line(input_with_overlay, ref0 - offset0, ref0 + offset0, cv::Scalar(0,255,0), 1, CV_AA);
        }*/
    //END OF LINE TRANSFORM WITH SAM'S HOUGH TRANSFORM

        //NamedImage(face_mask, "face_mask").show();


        /*cv::Mat mag, dir;

        sobel(input_gray, mag, dir, 5);

        cv:: Mat thresholded_mag;

        dynamicThreshold(mag, thresholded_mag, MAG_THRESHOLD, 255.0, CV_8U);

        // Remove lines

        cv::Mat line_hough_space(HOUGH_LINE_RHO_RESOLUTION, HOUGH_LINE_THETA_RESOLUTION, CV_32S, cv::Scalar(0));

        std::vector<cv::Vec3d> lines = houghLine(thresholded_mag, dir, line_hough_space, HOUGH_LINE_THRESHOLD, HOUGH_LINE_DIRRANGE);

        cv::Mat mag_mask(thresholded_mag.size(), CV_8U, cv::Scalar(255));

        for (const cv::Vec3d &line: lines)
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

            cv::line(mag_mask, ref - offset, ref + offset, cv::Scalar(0), 7, CV_AA);
        }

        for (const cv::Rect &face: prunedFaceDetections)
        {
            cv::rectangle(mag_mask, face, cv::Scalar(255), CV_FILLED);
        }

        cv::Mat masked_mag;

        thresholded_mag.copyTo(masked_mag, mag_mask);\*/

    //THIS SECTION PERFORMS SAM'S CIRCLE HOUGH TRANSFORM
        /*std::vector<int> hough_space_size = {HOUGH_CIRCLE_R_RESOLUTION, HOUGH_CIRCLE_AB_RESOLUTION, HOUGH_CIRCLE_AB_RESOLUTION};

        cv::Mat hough_space(hough_space_size.size(), &(hough_space_size[0]), CV_32S, cv::Scalar(0));

        cv::Vec2i minRect, maxRect;

        rectRange(prunedFaceDetections, minRect, maxRect);

        double rMin = HOUGH_RMIN_RATIO * (minRect[0] < minRect[1] ? minRect[0] : minRect[1]) / 2.0;

        double rMax = HOUGH_RMAX_RATIO * (minRect[0] > minRect[1] ? minRect[0] : minRect[1]) / 2.0;

        std::vector<cv::Vec4d> circles = houghCircle(masked_mag, dir, hough_space, rMin, rMax, HOUGH_CIRCLE_THRESHOLD);

        std::vector<cv::Vec4d> filtered_circles = filterList(circles, prunedFaceDetections, FILTER_MINDISTANCE);

        for (auto &circle: circles)
        {
            cv::circle(input_with_overlay, cv::Point(circle[0], circle[1]), circle[2], cvScalar(255, 0, 255), 2);
        }

        for (auto &circle: filtered_circles)
        {
            cv::circle(input_with_overlay, cv::Point(circle[0], circle[1]), circle[2], cvScalar(0, 255, 0), 2);
        }*/
    //END OF SAM'S HOUGH

    //START OF OPENCV CIRCLE HOUGH TRANSFORM

        //cv::GaussianBlur( face_mask_source, face_mask_source, Size(9, 9), 2, 2 );
        std::vector<cv::Vec3f> opencvcircles;
        int imagesplit = face_mask_source.cols;
        double threshy = 0.1;
        if(face_mask_source.rows > face_mask_source.cols) imagesplit = face_mask_source.rows; threshy = 0.7;
        cv::HoughCircles( face_mask, opencvcircles, CV_HOUGH_GRADIENT, 1, imagesplit*threshy, 100, 30, 20, 70 );
        for( std::size_t i = 0; i < opencvcircles.size(); i++ )
        {
          cv::Point center(cvRound(opencvcircles[i][0]), cvRound(opencvcircles[i][1]));
          int radius = cvRound(opencvcircles[i][2]);

          circle( input_with_overlay, center, radius, Scalar(0,0,255), 3, 8, 0 );
        }
    //END OF OPENCV HOUGH CIRCLE TRANSFORM

    //THIS SECTION CHECKS TO SEE IF A FACE HAS A CIRCLE CENTER IN IT, AND IF IT DOES, IT RUNS THE LINE DETECTOR ON IT AND DRAWS THE LINES MATE...
        //cv::Mat face_mask_source = imread(argv[imageNum], CV_LOAD_IMAGE_GRAYSCALE);
        //int i=0;
        //for (Rect detection : prunedFaceDetections)
        for( std::size_t i = 0; i < opencvcircles.size(); i++ )
        {
            cv::Rect detection = prunedFaceDetections[closestRect( opencvcircles[i][0], opencvcircles[i][1], prunedFaceDetections)];
            std::cout << "closest face detection index = " << prunedFaceDetections[ closestRect( opencvcircles[i][0], opencvcircles[i][1], prunedFaceDetections) ] << std::endl ;
            cv::rectangle(input_with_overlay, detection, cv::Scalar( 255, 255, 255 ), 2);

                ///////////////////////////////////////////
                cv::Mat faceRect = face_mask_source(detection).clone();
                cv::Mat softCopy = input_with_overlay(detection);

                cv::Scalar rectMean, rectStddev;
                cv::meanStdDev(faceRect, rectMean, rectStddev);  //find the mean and stddev of the image
                double rectSTD = rectStddev[0];
                std::cout << "standard deviation of the 'face' that has a circle in it... " << rectSTD << std::endl;

                //cv::Mat maskRect = face_mask(detection);
                //inputRect.copyTo(maskRect);
                vector<Vec2f> opencvlines;
                blur( faceRect, faceRect, Size(3,3) );
                Canny(faceRect, faceRect, 50, 150, 3);
                HoughLines(faceRect, opencvlines, 1, CV_PI/512, 80, 0, 0 );
                cvtColor(faceRect,faceRect, CV_GRAY2BGR);
                for( size_t i = 0; i < opencvlines.size(); i++ )
                {
                  float rho0 = opencvlines[i][0], theta0 = opencvlines[i][1];
                  Point pt1, pt2;
                  double a = cos(theta0), b = sin(theta0);
                  double x0 = a*rho0, y0 = b*rho0;
                  pt1.x = cvRound(x0 + 1000*(-b));
                  pt1.y = cvRound(y0 + 1000*(a));
                  pt2.x = cvRound(x0 - 1000*(-b));
                  pt2.y = cvRound(y0 - 1000*(a));
                  line( softCopy, pt1, pt2, Scalar(0,255,0), 1, CV_AA);
                }
                //Now we need to find the intersections of those lines... First loop through and make a fookin vector of intersections mate...
                std::vector<LineIntersection> intersections;
                //for( size_t i = 0; i < opencvlines.size(); i++ )
                //{
                  //float rho0 = opencvlines[i][0], theta0 = opencvlines[i][1];
                  intersections = LineIntersection::fromLines(opencvlines, 5.0,1);
                  for(LineIntersection intersection : intersections)
                  {
                      cv::circle( input_with_overlay, cv::Point(intersection.position[0],intersection.position[1]), 2, Scalar(0,0,255), 3, 8, 0 );
                      std::cout << intersection.position << std::endl;
                  }
                std::cout << "The intersections list has " << intersections.size() << "members..." << std::endl;

                std::cout << "The number of lines is!: " << opencvlines.size() << std::endl;


        }
    //END OF SECTION THAT DRAWS LINES IN FACES THAT HAVE A CIRCLE IN THEM...

        std::stringstream name;
        name << "lines+circles+pruningtest-" << imageID << ".jpg";



        NamedImage::showMany(std::vector<NamedImage>{
                                NamedImage(input_with_overlay, "Circles"),
                                 //NamedImage(masked_mag, "Lines Removed"),
                                 //NamedImage(mag_mask, "Mask"),
                                 //NamedImage(thresholded_mag, "Thresholded Mag"),
                                 //NamedImage(mag, "Mag")
                             });

        //imwrite(name.str(), input_with_overlay);
    }

    return 0;
}

bool containedInRect(cv::Rect rect, int x, int y)
{
    if( x > rect.x && x < rect.x + rect.width && y > rect.y && y < rect.y + rect.height) return true;
    else return false;
}

bool isAroundCircle(cv::Rect rect, std::vector<cv::Vec3f> & opencvcircles)
{
    for( std::size_t i = 0; i < opencvcircles.size(); i++ )
    {
        if(containedInRect(rect, opencvcircles[i][0], opencvcircles[i][1] )) return true;
    }
    return false;
}

int closestRect(int x, int y, std::vector<cv::Rect> & detections )
{
    cv::Point centre(x,y);
    double distance = std::numeric_limits<double>::max();
    int index;
    for(unsigned int detection = 0; detection < detections.size() ; detection++ )
    {
        double dist = euclideanDist( centre , Point(detections[detection].x + detections[detection].width/2, detections[detection].y + detections[detection].height/2));
        if( dist < distance )
        {
            distance = dist;
            index = detection;
        }
    }
    return index;
}

double euclideanDist(Point p, Point q) {
    Point diff = p - q;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}

