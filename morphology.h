//
// Created by papdawin on 2022. 11. 15..
//

#ifndef PROJECTNAME_MORPHOLOGY_H
#define PROJECTNAME_MORPHOLOGY_H

using namespace std;
using namespace cv;

#define PCB "/Users/pdwn/Desktop/pcb-hibas-8bpp.bmp"
#define PCB0 "/Users/pdwn/Desktop/pcb-hibas-8bpp0.bmp"

namespace Morphology{
    Mat dilation(Mat kep, int r=1){
        Mat out = Mat::zeros(kep.rows, kep.cols, CV_8UC1);
        Mat kernel = Mat::zeros(2*r+1,2*r+1,CV_8UC1);
        for (int kepRow = 0; kepRow < kep.rows; ++kepRow)
            for (int kepCol = 0; kepCol < kep.cols; ++kepCol)
                if((kepRow>r && kepCol>r) && (kepRow<kep.rows-r && kepCol<kep.cols-r)){
                    bool black=false;
                    for (int kerRow = 0; kerRow < kernel.rows; ++kerRow)
                        for (int kerCol = 0; kerCol < kernel.cols; ++kerCol){
                            int kepPix = kep.at<uchar>(kepRow-r+kerRow,kepCol-r+kerCol);
                            int kerPix = kernel.at<uchar>(kerRow,kerCol);
                            if((kepPix & kerPix) == kepPix )
                                black=true;
                        }
                    black ? out.at<uchar>(kepRow,kepCol)=0 : out.at<uchar>(kepRow,kepCol)=255;
                }
        return out;
    }
    Mat erosion(Mat kep, int r=1){
        Mat out = Mat::zeros(kep.rows, kep.cols, CV_8UC1);
        Mat kernel = Mat::zeros(2*r+1,2*r+1,CV_8UC1);
        for (int kepRow = 0; kepRow < kep.rows; ++kepRow)
            for (int kepCol = 0; kepCol < kep.cols; ++kepCol)
                if((kepRow>r && kepCol>r) && (kepRow<kep.rows-r && kepCol<kep.cols-r)){
                    bool white=false;
                    for (int kerRow = 0; kerRow < kernel.rows; ++kerRow)
                        for (int kerCol = 0; kerCol < kernel.cols; ++kerCol){
                            int kepPix = kep.at<uchar>(kepRow-r+kerRow,kepCol-r+kerCol);
                            int kerPix = kernel.at<uchar>(kerRow,kerCol);
                            if(kepPix | kerPix)
                                white=true;
                        }
                    white ? out.at<uchar>(kepRow, kepCol)=255 : out.at<uchar>(kepRow, kepCol)=0;
                }
        return out;
    }
    Mat diff(Mat a, Mat b){
        Mat out = Mat::zeros(a.rows, a.cols, CV_8UC1);
        out.forEach<uchar>([&](uchar &pixel, const int pos[]){
            if(a.at<uchar>(pos[0],pos[1]) == b.at<uchar>(pos[0],pos[1]))
                pixel=255;
            else
                pixel=0;
        });
        return out;
    }

    void run(){
        Mat gsrc;
        cvtColor(cv::imread(PCB), gsrc, cv::COLOR_BGR2GRAY);
        imshow("original", gsrc);
        Mat ero = erosion(gsrc);
        imshow("erosion",ero);
        Mat dil = dilation(gsrc);
        imshow("dilation",dil);
        Mat open = dilation(ero);
        Mat close = erosion(dil);
        imshow("open",open);
        imshow("close",close);
        imshow("openDif", diff(gsrc,open));
        imshow("colseDif", diff(gsrc,close));
    }
}

#endif //PROJECTNAME_MORPHOLOGY_H
