//
// Created by papdawin on 2022. 10. 18..
//

#ifndef PROJECTNAME_TRANSFORMATIONS_H
#define PROJECTNAME_TRANSFORMATIONS_H

#include "misc.h" //helper functions

#define DOGE "/Users/pdwn/Desktop/doge.jpeg"
#define PEPPERS "/Users/pdwn/Desktop/peppers_vilagos.bmp"
#define AIRPLANE "/Users/pdwn/Desktop/airplane.bmp"
#define DOGGO "/Users/pdwn/Desktop/doggo.jpg"

using namespace std;
using namespace cv;

namespace HistogramTransformations{
    Mat linearis(Mat kep, int min, int max){
        Mat ret = kep.clone();
        for (int i = 0; i < kep.rows; ++i)
            for (int j = 0; j < kep.cols; j++)
                ret.at<uchar>(i,j)=255*(kep.at<uchar>(i,j)-min)/(max-min);
        return ret;
    }
    Mat gyokos(Mat kep, int min, int max){
        Mat ret = kep.clone();
        for (int i = 0; i < kep.rows; ++i)
            for (int j = 0; j < kep.cols; j++)
                ret.at<uchar>(i,j)=255*sqrtf((kep.at<uchar>(i,j)-min)/(max-min*1.0f));
        return ret;
    }
    Mat negyzetes(Mat kep, int min, int max){
        Mat ret = kep.clone();
        for (int i = 0; i < kep.rows; ++i)
            for (int j = 0; j < kep.cols; j++)
                ret.at<uchar>(i,j)=255*pow((kep.at<uchar>(i,j)-min)/(max-min*1.0f),2);
        return ret;
    }
    Mat kiegyenlites(Mat kep, Mat hist, int K){
        int N = kep.rows*kep.cols;
        float unitSize = N / K;
        vector<int> LUT(256);

        int sum=0, i=0;
        for (int j = 0; j < LUT.size(); j++) {
            if(sum < unitSize)
                sum+=hist.at<int>(j,0);
            else{ sum=0; i++; }
            LUT[j]=i*LUT.size()/K;
        }
        //for_each(LUT.begin(), LUT.end(),[](int a){cout<<a<<", ";});
        for (int i = 0; i < kep.rows; ++i)
            for (int j = 0; j < kep.cols; ++j)
                kep.at<uchar>(i,j)=LUT[kep.at<uchar>(i,j)];
        return kep;
    }

    void hisztogramSzethuzas() {
        int min=0,max=256;
        Mat gsrc;
        cvtColor(cv::imread(PEPPERS), gsrc, cv::COLOR_BGR2GRAY);
        imshow("before", gsrc);
        Mat hist = misc::computeHistogram(gsrc);
        misc::displayHistogram("Eredeti",misc::computeHistogram(gsrc),min,max);
        //Lineáris
        Mat lin = linearis(gsrc,min,max);
        imshow("linearis",lin);
        hist=misc::computeHistogram(lin);
        min = misc::histVegpont(hist,"min");
        max = misc::histVegpont(hist,"max");
        misc::displayHistogram("linearis",hist,min, max);
        //Gyökös
        Mat gyok = gyokos(gsrc,min,max);
        imshow("gyokos",gyok);
        hist=misc::computeHistogram(gyok);
        min = misc::histVegpont(hist,"min");
        max = misc::histVegpont(hist,"max");
        misc::displayHistogram("gyokos",hist,min, max);
        //Négyzetes
        Mat negy = negyzetes(gsrc,min,max);
        imshow("negyzetes",negy);
        hist=misc::computeHistogram(negy);
        min = misc::histVegpont(hist,"min");
        max = misc::histVegpont(hist,"max");
        misc::displayHistogram("negyzetes",hist,min, max);
    }
    void hisztogramKiegyenlites(){
        Mat gsrc;
        cvtColor(cv::imread(AIRPLANE), gsrc, cv::COLOR_BGR2GRAY);
        imshow("original", gsrc);
        Mat hist = misc::computeHistogram(gsrc);
        misc::displayHistogram("original",hist.clone());
        Mat kiegy = kiegyenlites(gsrc,hist,2);
        imshow("kiegyenlitett", kiegy);
        misc::displayHistogram("kiegy", misc::computeHistogram(kiegy));
    }
}

#endif //PROJECTNAME_TRANSFORMATIONS_H
