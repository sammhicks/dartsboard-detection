#include "calcf1.h"

double calcf1(std::vector<cv::Rect> &groundTruth, std::vector<cv::Rect> &faces, int trueNumberOfBoards)
{
    int tp = 0;
    int othertruedetections = 0;
    truePositive(groundTruth, faces, tp, othertruedetections);
    //std::cout << "true positive score: " << tp << std::endl;

    if(!tp) return 0.0;

    int fp = faces.size() - tp - othertruedetections;

    double precision = tp / (double)(tp + fp);
    double recall =    tp / (double)(trueNumberOfBoards);
    return 2 * (precision * recall) / (precision + recall);
}


void truePositive(std::vector<cv::Rect> &groundTruth, std::vector<cv::Rect> &faces, int &total, int &othertrue)
{
    cv::Rect intersection;
    double ratio;

    for (cv::Rect g : groundTruth)
    {
        int extra = 0;

        for(cv::Rect f : faces)
        {
            intersection = g & f;

            if(intersection.width && intersection.height)
            {
                ratio = (intersection.width*intersection.height)/(double)( g.width*g.height + f.width*f.height - (intersection.width*intersection.height));
                bool isCentreFine = centresThresh(g, f, F1_THRESHOLD/2);
                double variableThresh = F1_THRESHOLD;
                if(isCentreFine) variableThresh = 0.2;
                if (ratio >= variableThresh && isCentreFine)
                {
                    if (extra == 0)
                    {
                        //std::cout << "We're in here so fucking tp should definitely get incremented!" << std::endl;
                        total++;
                    }

                    extra++;
                }
            }
        }
        if (extra > 0) othertrue += extra - 1;
    }
}

bool centresThresh(cv::Rect g, cv::Rect f, double thresh){
    cv::Point gc(g.x + g.width/2, g.y + g.height/2);
    cv::Point fc(f.x + f.width/2, f.y + f.height/2);

    double dx = (fc.x - gc.x) / g.width;
    double dy = (fc.y - gc.y) / g.height;

    return (dx * dx + dy * dy) < (thresh * thresh);
}
