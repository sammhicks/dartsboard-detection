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
void detectAndDisplay( Mat frame, vector<Rect> & ground, int trueNumber );
void pruneFaces(vector<Rect> &faces, vector<Rect> &prunedFaces, double threshold);

double calcf1(vector<Rect> &groundTruth, vector<Rect> &faces, double threshold, int trueNumberOfBoards);
void truePositive(vector<Rect> &groundTruth, vector<Rect> &faces, double threshold, int & total, int & othertruedetections);
bool centresThresh(Rect g, Rect f, double thresh);

/** Global variables */
String cascade_name = "../dartcascade/cascade.xml";
CascadeClassifier cascade;
Mat frame;
Mat unchanged;


/** @function main */
int main( int argc, const char** argv )
{
    if( !cascade.load( cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

    //Mat frame;  //Frame is now global so it can be easily accessed inside of the other functions...
    string name;
    stringstream ss;

    for(int i=1;i<=16;i++){ // Use this loop to iterate through all of the training images.

        ss.str("");
        ss << i-1;
        name = "pruneFACEStest"+ss.str()+".jpg";
        //cout << name;
        frame = imread(argv[i], CV_LOAD_IMAGE_COLOR);
        unchanged = frame.clone();
        cout << "-----------dart " << i-1 << "--------------" << endl;
        detectAndDisplay( frame, dartsgt[i-1], dartnumbersgt[i-1] );
        imwrite( name, frame );
    }
//imwrite( "TEST.jpg", frame );

	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame , vector<Rect> & ground, int trueNumber)
{
	std::vector<Rect> faces;
    std::vector<Rect> prunedFaces;
	Mat frame_gray;

	// 1. Prepare Image by turning it into Grayscale and normalising lighting
	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	// 2. Perform Viola-Jones Object Detection 
    cascade.detectMultiScale( frame_gray, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );

    pruneFaces(faces, prunedFaces, 0.75);

       // 4. Draw box around faces found
    for( int i = 0; i < prunedFaces.size(); i++ )
	{
        rectangle(frame, Point(prunedFaces[i].x, prunedFaces[i].y), Point(prunedFaces[i].x + prunedFaces[i].width, prunedFaces[i].y + prunedFaces[i].height), Scalar( 0, 255, 0 ), 2);
    }
    cout << "f1 score: " << calcf1(ground, prunedFaces, 0.5, trueNumber);
}

double calcf1(vector<Rect> &groundTruth, vector<Rect> &faces, double threshold, int trueNumberOfBoards)
{
    int tp = 0;
    int othertruedetections = 0;
    truePositive(groundTruth,faces,threshold, tp, othertruedetections);

    if(!tp) return 0.0;

    int fp = faces.size() - tp - othertruedetections;

    double precision = tp / (double)(tp + fp);
    double recall =    tp / (double)(trueNumberOfBoards);
    return 2 * (precision * recall) / (precision + recall);
}

void pruneFaces(vector<Rect> &faces, vector<Rect> &prunedFaces, double threshold)
{
    Mat tempMat;
    Mat frame_gray;
    cvtColor( unchanged, frame_gray, CV_BGR2GRAY ); //convert the original image to grayscale

    Scalar mean, stddev;
    meanStdDev(frame_gray, mean, stddev);  //find the mean and stddev of the image
    double frameMean = mean[0];
    double frameSTD = stddev[0];

    for( Rect f : faces)
    {
        tempMat = frame_gray(Rect(f.x, f.y, f.width, f.height) );
        meanStdDev(tempMat, mean, stddev);

        if(stddev[0] /frameSTD > threshold){ //IF the proportion of the stddev of the detection to the stddev of the whole image is above the threshold then it is considered a valid detection
            rectangle(frame, Point(f.x, f.y), Point(f.x + f.width, f.y + f.height), Scalar( 0, 0, 0 ), 2);
            prunedFaces.push_back(Rect(f.x, f.y, f.width, f.height));
        }
    }
}

void truePositive(vector<Rect> &groundTruth, vector<Rect> &faces, double threshold, int & total, int & othertrue)
{
    Rect intersection;
    double ratio;

    for (Rect g : groundTruth)
    {
        int extra = 0;
        rectangle(frame, Point(g.x, g.y), Point(g.x + g.width, g.y + g.height), Scalar( 255, 255, 255 ), 2);
        for(Rect f : faces)
        {
            intersection = g & f;

            if(intersection.width && intersection.height)
            {
                ratio = (intersection.width*intersection.height)/(double)( g.width*g.height + f.width*f.height - (intersection.width*intersection.height));
                bool isCentreFine = centresThresh(g,f,threshold);
                if(ratio >= threshold && !extra && isCentreFine)
                {
                    rectangle(frame, Point(f.x, f.y), Point(f.x + f.width, f.y + f.height), Scalar( 0, 0, 255 ), 2);
                    total++;
                    extra++; //should this be here?!?!?!?!? maybe not?!?!?!?!
                }
                else if(ratio >= threshold && isCentreFine)
                {
                    extra++;
                }
            }
        }
        if(extra) othertrue += extra-1;
    }
}

bool centresThresh(Rect g, Rect f, double thresh){
    Point gc(g.x + g.width/2, g.y + g.height/2);
    Point fc(f.x + f.width/2, f.y + f.height/2);

    double dx = (fc.x - gc.x) / g.width;
    double dy = (fc.y - gc.y) / g.height;

    return (dx * dx + dy * dy) < (thresh * thresh);
}
