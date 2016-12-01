/////////////////////////////////////////////////////////////////////////////
//
// COMS30121 - face.cpp
//
/////////////////////////////////////////////////////////////////////////////

// header inclusion
// header inclusion
#include <stdio.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

#include "sobel.h"
#include "hough.h"
#include "filter_list.h"
#include "namedimage.h"

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
String cascade_name = "../dartcascade/cascade.xml";
CascadeClassifier cascade;


/** @function main */
int main( int argc, const char** argv )
{

    if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    Mat frame;
    // 1. Read Input Image
    string name;
    stringstream ss;
    frame = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    /*for(int i=1;i<=16;i++){

        ss.str("");
        ss << i-1;
        name = "p2detected"+ss.str()+".jpg";
        cout << name;
        frame = imread(argv[i], CV_LOAD_IMAGE_COLOR);
        detectAndDisplay( frame );
        imwrite( name, frame );
    }*/

    // 2. Load the Strong Classifier in a structure called `Cascade'
    //if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

    // 3. Detect Faces and Display Result
    //detectAndDisplay( frame );

    // 4. Save Result Image
    //imwrite( "detected.jpg", frame );

    //namedWindow("frame", WINDOW_AUTOSIZE);
    //imshow("frame", frame);

    cv::Mat mag, dir, thresholded_mag;

    sobel(frame, mag, dir, 5);

    NamedImage::showImage(NamedImage(mag, "Mag"));

    std::vector<int> hough_space_size = {64, 256, 256};

    cv::Mat hough_space(hough_space_size.size(), &(hough_space_size[0]), CV_32S);

    auto circles = hough(mag, dir, thresholded_mag, hough_space, 10, 200, 64.0, 0.3);

    NamedImage::showImage(NamedImage(thresholded_mag, "Thresholded Mag"));

    auto filtered_circles = filter_list(circles, 50);

    cv::Mat circles_with_overlay;

    cv::cvtColor(frame, circles_with_overlay, cv::COLOR_GRAY2RGB);

    cv::Mat filtered_circles_with_overlay = circles_with_overlay.clone();

    for (auto &circle: circles)
    {
        cv::circle(circles_with_overlay, cv::Point(circle[0], circle[1]), circle[2], cvScalar(0, 255, 0));
    }

    for (auto &circle: filtered_circles)
    {
        cv::circle(filtered_circles_with_overlay, cv::Point(circle[0], circle[1]), circle[2], cvScalar(0, 255, 0));
    }

    std::vector<NamedImage> images = {
        NamedImage(circles_with_overlay, "Circles"),
        NamedImage(filtered_circles_with_overlay, "Filtered Circles")
    };

    NamedImage::showImages(images);

    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat frame_gray;

    // 1. Prepare Image by turning it into Grayscale and normalising lighting
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );

    // 2. Perform Viola-Jones Object Detection
    cascade.detectMultiScale( frame_gray, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );
    // 3. Print number of Faces found
    std::cout << faces.size() << std::endl;

    // 4. Draw box around faces found
    for( int i = 0; i < faces.size(); i++ )
    {
        rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar( 0, 255, 0 ), 2);
    }
    printf("dickhead!\n");
}
