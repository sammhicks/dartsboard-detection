#include "detectanddisplay.h"

std::vector<cv::Rect> detectAndDisplay(cv::Mat &input, cv::CascadeClassifier &cascade, std::vector<cv::Rect> &ground, int trueNumber)
{
    // 1. Prepare Image by normalising lighting
    cv::Mat input_equalised;

    cv::equalizeHist(input, input_equalised);

    // 2. Perform Viola-Jones Object Detection
    std::vector<cv::Rect> faces;

    cascade.detectMultiScale( input_equalised, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, cv::Size(50, 50), cv::Size(500,500) );
    //std::cout << faces.size();
    std::vector<cv::Rect> prunedFaces = pruneFaces(input, faces);

       // 4. Draw box around faces found
    /*for( unsigned int i = 0; i < prunedFaces.size(); i++ )
    {
        cv::rectangle(input, cv::Point(prunedFaces[i].x, prunedFaces[i].y), cv::Point(prunedFaces[i].x + prunedFaces[i].width, prunedFaces[i].y + prunedFaces[i].height), cv::Scalar( 0, 255, 0 ), 2);
    }*/
    //std::cout << "f1 score: " << calcf1(ground, faces, trueNumber) << std::endl;
    std::cout << calcf1(ground, faces, trueNumber) << std::endl;

    return prunedFaces;
}

std::vector<cv::Rect> pruneFaces(cv::Mat &input, std::vector<cv::Rect> &faces)
{
    cv::Scalar mean, stddev;
    cv::meanStdDev(input, mean, stddev);  //find the mean and stddev of the image
    double frameMean = mean[0];
    double frameSTD = stddev[0];

    std::vector<cv::Rect> prunedFaces;

    for( cv::Rect f : faces)
    {
        cv::meanStdDev(input(cv::Rect(f.x, f.y, f.width, f.height)), mean, stddev);

        if(stddev[0] /frameSTD > PRUNING_THRESHOLD){ //IF the proportion of the stddev of the detection to the stddev of the whole image is above the threshold then it is considered a valid detection
            //cv::rectangle(input, cv::Point(f.x, f.y), cv::Point(f.x + f.width, f.y + f.height), cv::Scalar( 0, 0, 0 ), 2);
            prunedFaces.push_back(cv::Rect(f.x, f.y, f.width, f.height));
        }
    }

    return prunedFaces;
}
