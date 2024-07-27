//
// Created by papdawin on 2022. 10. 26..
//

#ifndef PROJECTNAME_CONVOLUTIONS_H
#define PROJECTNAME_CONVOLUTIONS_H

#include "transformations.h"
#include "misc.h"

using namespace std;
using namespace cv;

#define KESZ "/Users/pdwn/Desktop/keszHQ.jpg"
#define LENA "/Users/pdwn/Desktop/lena.bmp"


namespace Convolutions{
    Mat laplaceKernel(){
        return Mat_<double>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0;
    }
    Mat L1 (){
        return Mat_<double>(3,3) << 1, 2, 1, 2, 4, 2, 1, 2, 1;
    }
    Mat L2 (){
        return Mat_<double>(3,3) << 1, 0, -1, 2, 0, -2, 1, 0, -1;
    }
    Mat L3 (){
        return Mat_<double>(3,3) << -1, 2, -1, -2, 4, -2, -1, 2, -1;
    }
    Mat L4 (){
        return Mat_<double>(3,3) << -1, -2, -1, 0, 0, 0, 1, 2, 1;
    }
    Mat L5 (){
        return Mat_<double>(3,3) << 1, 0, -1, 0, 0, 0, -1, 0, 1;
    }
    Mat L6 (){
        return Mat_<double>(3,3) << -1 , 2, -1, 0, 0, 0, 1, -2, 1;
    }
    Mat L7 (){
        return Mat_<double>(3,3) << -1, -2, -1, 2, 4, 2, -1, -2, -1;
    }
    Mat L8 (){
        return Mat_<double>(3,3) << -1, 0, 1, 2, 0, -2, -1, 0, 1;
    }
    Mat L9 (){
        return Mat_<double>(3,3) << 1, -2, 1, -2, 4, -2, 1, -2, 1;
    }
    Mat boxBlur(int r){
        auto out = Mat_<double>(r*2+1, r*2+1);
        for (int i = 0; i < r*2+1; ++i) {
            for (int j = 0; j < r*2+1; ++j) {
                out.at<double>(i,j) = 1.f/pow(r*2+1,2);
            }
        }
        return out;
    }
    Mat specKernel(){
        return Mat_<double>(3,3) << 0.125, 0.125, 0.125, 0.125, 0, 0.125, 0.125, 0.125, 0.125;
    }
    Mat egysegKernel(){
        return Mat_<uchar>(3,3) << 1, 1, 1, 1, 1, 1, 1, 1, 1;
    }
    Mat convolution(Mat kep, Mat kernel){
        int kernelsugar=(kernel.rows-1)/2;
        int kepmagassaga=kep.rows, kepszelessege=kep.cols;
        Mat out = Mat::zeros(kep.rows, kep.cols, CV_8UC1);
        for(int i=0; i<kepmagassaga-(2*kernelsugar); i++)
            for(int j=0; j<kepszelessege-(2*kernelsugar); j++){
                float pixval=0;
                for(int k=0; k<2*kernelsugar+1; k++)
                    for(int l=0; l<2*kernelsugar+1; l++){
                        if(i==0||j==0||i==kepmagassaga-1||j==kepszelessege-1)
                            pixval+= 127*kernel.at<double>(k,l);
                        else
                            pixval+=(int)kep.at<uchar>(i+k,j+l)*kernel.at<double>(k,l);
                    }
                pixval = pixval<0 ? abs(pixval) : pixval;
                pixval = pixval>255 ? 255: pixval;
                out.at<uchar>(i+kernelsugar,j+kernelsugar) = pixval;
            }
        return out;
    }
    void laplaceFilter(){
        Mat gsrc;
        cvtColor(cv::imread(LENA), gsrc, cv::COLOR_BGR2GRAY);
        imshow("original", gsrc);
        misc::displayHistogram("original ",misc::computeHistogram(gsrc));
        gsrc = HistogramTransformations::linearis(gsrc,0,256);
        Mat out = convolution(gsrc, laplaceKernel());
        Mat hist = misc::computeHistogram(out);
        Mat kiegy = HistogramTransformations::gyokos(out,misc::histVegpont(hist,"min"),misc::histVegpont(hist,"max"));
        imshow("convolution",kiegy);
        misc::displayHistogram("convolution ",misc::computeHistogram(kiegy));
    }
    Mat horizontalSobel(){
        return Mat_<double>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1;
    }
    Mat verticalSobel(){
        return Mat_<double>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1;
    }
    Mat sobel(Mat a, Mat b){
        Mat out = Mat::zeros(a.rows, a.cols, CV_8UC1);
        out.forEach<uchar>([&](uchar &pixel, const int pos[]){
            int val = a.at<uchar>(pos[0],pos[1])+a.at<uchar>(pos[0],pos[1]);
            val = 255<val ? 255 : val;
            pixel=val;
        });
        return out;
    }

    void sobelFilter(){
        Mat gsrc;
        cvtColor(cv::imread(KESZ), gsrc, cv::COLOR_BGR2GRAY);
        Mat out = sobel(convolution(gsrc,horizontalSobel()),convolution(gsrc,verticalSobel()));
        imshow("sobel",out);
        misc::displayHistogram("convolution ",misc::computeHistogram(out));
    }
}
#endif //PROJECTNAME_CONVOLUTIONS_H
