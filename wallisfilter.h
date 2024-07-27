//
// Created by papdawin on 2022. 10. 26..
//

#ifndef PROJECTNAME_WALLISFILTER_H
#define PROJECTNAME_WALLISFILTER_H


using namespace std;
using namespace cv;

#define BRIDGE "/Users/pdwn/Desktop/bridge.bmp"
#define KESZ "/Users/pdwn/Desktop/keszHQ.jpg"

namespace WallisFilter{
    Mat lokAtlag(Mat kep, int r) {
        Mat kernel = Convolutions::boxBlur(r);
        Mat out = Convolutions::convolution(kep,kernel);
        return out;
    }
    Mat szoras(Mat kep, int r) {
        Mat out = Mat::zeros(kep.rows, kep.cols, CV_64F);
        Mat avg = lokAtlag(kep,r);
        for (int i = 0; i < out.rows; i++)
            for (int j = 0; j < out.cols; j++){
                float sum = 0;
                for (int row = i - r; row <= i + r; row++)
                    for (int col = j - r; col <= j + r; col++)
                        if(!(i < 0 || j < 0||i>out.cols||j>out.rows))
                            sum += pow(kep.at<uchar>(row, col) - avg.at<uchar>(row,col), 2);
                out.at<double>(i, j) = sum / pow((2 * r + 1), 2);
            }
        return out;
    }
    Mat Wallis(Mat kep, Mat avg, Mat szor, int M, int S, float A, float r) {
        Mat out = Mat::zeros(kep.rows, kep.cols, kep.type());
        for (int i = 0; i < out.rows; i++)
            for (int j = 0; j < out.cols; j++) {
                auto pixval = ((kep.at<uchar>(i, j) - avg.at<uchar>(i, j))
                               * ((A * S ) / (S + (A * sqrt(szor.at<double>(i, j))))))
                              + ((r * M ) + ((1.0f - r ) * avg.at<uchar>(i, j)));
                pixval = pixval<0 ? 0 : pixval;
                pixval = pixval>255 ? 255: pixval;
                out.at<uchar>(i,j) = pixval;
            }
        return out;
    }
    Mat wallisSzuro(Mat kep, int rad, int M=128, int S=100, float A=2.5f, float r=0.25f){
        Mat avg = lokAtlag(kep, rad);
        Mat szor = szoras(kep, rad);
        Mat wallis = Wallis(kep, avg, szor,M,S,A,r);
        return wallis;
    }

    void wallisFilter(){
        Mat gsrc;
        cvtColor(cv::imread(BRIDGE), gsrc, cv::COLOR_BGR2GRAY);
        imshow("original", gsrc);
        misc::displayHistogram("original ",misc::computeHistogram(gsrc));
        Mat light = wallisSzuro(gsrc,10,220,150,5,0.95);
        Mat dark = wallisSzuro(gsrc,10,40,150,5,0.95);
        imshow("wallis_dark",dark);
        imshow("wallis_light",light);
        misc::displayHistogram("wallis_dark",misc::computeHistogram(dark));
        misc::displayHistogram("wallis_light",misc::computeHistogram(light));
    }
}
#endif //PROJECTNAME_WALLISFILTER_H
