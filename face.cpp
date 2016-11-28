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
#include "hough.h"

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
    frame = imread(argv[1],0);
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
