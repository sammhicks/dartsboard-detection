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
#include "ground.h"

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( Mat frame, vector<Rect> & ground );

double calcf1(vector<Rect> &groundTruth, vector<Rect> &faces, double threshold, int trueNumberOfBoards);
int truePositive(vector<Rect> &groundTruth, vector<Rect> &faces, double threshold);

/** Global variables */
String cascade_name = "../dartcascade/cascade.xml";
CascadeClassifier cascade;


/** @function main */
int main( int argc, const char** argv )
{
    if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

    Mat frame;
    string name;
    stringstream ss;

    frame = imread(argv[11], CV_LOAD_IMAGE_COLOR);
    detectAndDisplay( frame, dartsgt[10] );
    //frame = imread(argv[1],0);

    /*for(int i=1;i<=16;i++){ // Use this loop to iterate through all of the training images.

        ss.str("");
        ss << i-1;
        name = "p2detected"+ss.str()+".jpg";
        cout << name;
        frame = imread(argv[i], CV_LOAD_IMAGE_COLOR);
        detectAndDisplay( frame );
        imwrite( name, frame );
    }*/


	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame , vector<Rect> & ground)
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

    //cout << "f1 for image 10 mate!" << calcf1(dartsgt[10], faces, 0.5, dartnumbersgt[10]);
    //cout << "true positive for image 10 mate!" << truePositive(dartsgt[10], faces, 0.5);
}

double calcf1(vector<Rect> &groundTruth, vector<Rect> &faces, double threshold, int trueNumberOfBoards)
{
    int tp = truePositive(groundTruth,faces,threshold);

    int fp = faces.size() - tp;

    double precision = tp / (double)(tp + fp);
    double recall =    tp / (double)(trueNumberOfBoards);

    return 2 * (precision * recall) / (precision + recall);
}

int truePositive(vector<Rect> &groundTruth, vector<Rect> &faces, double threshold)
{
    int total = 0;
    Rect intersection;
    double ratio;

    for (Rect g : groundTruth)
    {

        for(Rect f : faces)
        {

            intersection = g & f;

            if(intersection.width && intersection.height)
            {
                ratio = (intersection.width*intersection.height)/(double)( g.width*g.height + f.width*f.height - (intersection.width*intersection.height));

                if(ratio >= threshold)
                {
                    total++;
                    break;
                }
            }
        }
    }
    return total;
}
