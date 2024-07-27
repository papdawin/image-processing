//
// Created by papdawin on 2022. 10. 26..
//

#ifndef PROJECTNAME_NONLINEAR_H
#define PROJECTNAME_NONLINEAR_H


using namespace std;
using namespace cv;

#define SALTPEPPER "/Users/pdwn/Desktop/0.1.bmp"
#define GAUSS "/Users/pdwn/Desktop/0.025.bmp"
#define KESZ "/Users/pdwn/Desktop/keszHQnoisy.jpg"

namespace nonLinearFilter{
    Mat outlier(Mat kep, int th=30){
        Mat kernel = Convolutions::specKernel();
        Mat out = Mat::zeros(kep.rows, kep.cols, CV_8UC1);
        Mat lp = Convolutions::convolution(kep, kernel);
        kep.forEach<uchar>([&](uchar &pixel, const int pos[]){
            auto convPix = lp.at<uchar>(pos[0],pos[1]);
            if((abs(convPix-pixel))<th)
                out.at<uchar>(pos[0],pos[1])=pixel;
            else
                out.at<uchar>(pos[0],pos[1])=convPix;
        });
        return out;
    }
    Mat median(Mat kep, int r){
        Mat out = Mat::zeros(kep.rows, kep.cols, CV_8UC1);
        kep.forEach<uchar>([&](uchar &pixel, const int pos[]){
            vector<uchar> values;
            if(pos[0]<r || pos[1]<r || pos[0]>kep.rows-r || pos[1]>kep.cols-r)
                values.push_back(pixel);
            else
                for (int i = pos[0]-r; i < pos[0]+r; ++i)
                    for (int j = pos[1]-r; j < pos[1]+r; ++j)
                        values.push_back(kep.at<uchar>(i,j));
            std::sort(values.begin(), values.end());
            out.at<uchar>(pos[0],pos[1]) = values.at(values.size()/2);
        });
        return out;
    }
    Mat fastMedian(Mat kep, int r){
        Mat out = Mat::zeros(kep.rows, kep.cols, CV_8UC1);
        kep.forEach<uchar>([&](uchar &pixel, const int pos[]){
            if(!(pos[0]<r || pos[1]<r || pos[0]>kep.rows-r || pos[1]>kep.cols-r)){
                vector<uchar> row;
                for (int i = pos[0]-r; i < pos[0]+r; ++i){
                    vector<uchar> col;
                    for (int j = pos[1]-r; j < pos[1]+r; ++j){
                        col.push_back(kep.at<uchar>(i,j));
                    }
                    std::sort(col.begin(), col.end());
                    row.push_back(col.at(col.size()/2));
                }
            std::sort(row.begin(), row.end());
            out.at<uchar>(pos[0],pos[1]) = row.at(row.size()/2);
            }else
                out.at<uchar>(pos[0],pos[1])=pixel;
        });
        return out;
    }
    void nonLinearFilter(){
        Mat gsrc;
        cvtColor(cv::imread(GAUSS), gsrc, cv::COLOR_BGR2GRAY);
        imshow("original", gsrc);
        misc::displayHistogram("original ",misc::computeHistogram(gsrc));
        imshow("outlier", outlier(gsrc));
        misc::displayHistogram("outlier ",misc::computeHistogram(outlier(gsrc)));
        imshow("median", median(gsrc, 3));
        imshow("fastmedian", fastMedian(gsrc, 3));
        misc::displayHistogram("median ",misc::computeHistogram(median(gsrc, 3)));
    }
}

#endif //PROJECTNAME_NONLINEAR_H
