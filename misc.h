//
// Created by papdawin on 2022. 10. 21..
//

#ifndef PROJECTNAME_MISC_H
#define PROJECTNAME_MISC_H

using namespace std;
using namespace cv;

namespace misc {
    Mat computeHistogram(Mat kep) {
        Mat histogram = Mat::zeros(256, 1, CV_32SC1);

        for (int i = 0; i < kep.rows; i++)
            for (int j = 0; j < kep.cols; j++) {
                //cout<<(int)kep.at<uchar>(i, j)<<", ";
                histogram.at<int>((int)kep.at<uchar>(i, j)-1, 0) += 1;
            }
        return histogram;
    }
    int histVegpont(Mat hist, string minmax){
        if (minmax == "min") {
            for(int i = 0; i < hist.rows; i++)
                if ((int)hist.at<uchar>(i,0)!=0)
                    return i;
        }
        for(int i = hist.rows; i > 0; i--)
            if ((int)hist.at<uchar>(i,0)!=0)
                return i;
    }
    void displayHistogram(string name, Mat hist, int histStart=0, int histEnd=255){
        Mat copy=hist.clone();
        int hist_w = 512, hist_h = 400;
        int bin_w = cvRound((double)hist_w / histEnd);
        Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
        normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
        for (int i = histStart; i < histEnd; ++i)
            line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(hist.at<int>(i - 1))),
                 Point(bin_w * (i), hist_h - cvRound(hist.at<int>(i))),
                 Scalar(255, 255,255), 2, 8, 0);
        imshow(name+" histogram",histImage);
    }
}

#endif //PROJECTNAME_MISC_H
