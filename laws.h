//
// Created by papdawin on 2022. 11. 29..
//

#ifndef PROJECTNAME_LAWS_H
#define PROJECTNAME_LAWS_H

#define INPUT "/Users/pdwn/Desktop/laws_input.bmp"
#define TEXTURE "/Users/pdwn/Desktop/laws_texture.bmp"
#define INPUT0 "/Users/pdwn/Desktop/laws_input0.bmp"
#define TEXTURE0 "/Users/pdwn/Desktop/laws_texture0.bmp"

namespace laws {
    list<Mat> calcConv(Mat kep) {
        list<Mat> convolutions;
        convolutions.push_back(Convolutions::convolution(kep, Convolutions::L1() / 36));
        convolutions.push_back(Convolutions::convolution(kep, Convolutions::L2() / 12));
        convolutions.push_back(Convolutions::convolution(kep, Convolutions::L3() / 12));
        convolutions.push_back(Convolutions::convolution(kep, Convolutions::L4() / 12));
        convolutions.push_back(Convolutions::convolution(kep, Convolutions::L5() / 4));
        convolutions.push_back(Convolutions::convolution(kep, Convolutions::L6() / 4));
        convolutions.push_back(Convolutions::convolution(kep, Convolutions::L7() / 4));
        convolutions.push_back(Convolutions::convolution(kep, Convolutions::L8() / 4));
        convolutions.push_back(Convolutions::convolution(kep, Convolutions::L9() / 4));
        return convolutions;
    }
    Mat absoluteDif(Mat textureEnergy, Mat inputEnergy) {
        int rows = inputEnergy.rows;
        int cols = inputEnergy.cols;
        Mat res = inputEnergy.clone();
        res = Scalar::all(0);
        for (int k = 0; k < 9; k++)
            for (int i = 0; i < cols; i++)
                for (int j = 0; j < rows; j++)
                    res.at<double>(i, j) += abs(textureEnergy[k] - inputEnergy[k].at<double>(i, j));
        return res;
    }

    void minSearch(Mat &textureAbs, Mat &textureAbs2, Mat &textureAbs3, Mat &textureAbs4, Mat &res);

    Mat laws(Mat texture1, Mat texture2, Mat texture3, Mat texture4, Mat inputEnergy) {
        Mat textureAbs = absoluteDif(texture2, inputEnergy);
        Mat textureAbs2 = absoluteDif(texture1, inputEnergy);
        Mat textureAbs3 = absoluteDif(texture3, inputEnergy);
        Mat textureAbs4 = absoluteDif(texture4, inputEnergy);

        Mat res(inputEnergy[0].rows, inputEnergy[0].cols, CV_8UC1);

        minSearch(textureAbs, textureAbs2, textureAbs3, textureAbs4, res);
        return res;
    }

    void minSearch(Mat &textureAbs, Mat &textureAbs2, Mat &textureAbs3, Mat &textureAbs4, Mat &res) {
        for (int i = 0; i < res.cols; i++) {
            for (int j = 0; j < res.rows; j++) {
                int min_idx = 0;
                double absDifference[4] = {textureAbs2.at<double>(i, j), textureAbs.at<double>(i, j), textureAbs3.at<double>(i, j), textureAbs4.at<double>(i, j) };
                for (int i = 1; i < 4; i++)
                    if (absDifference[i] < absDifference[min_idx])
                        min_idx = i;
                if (min_idx == 0)
                    res.at<unsigned char>(i, j) = 255;
                else if (min_idx == 1)
                    res.at<unsigned char>(i, j) = 85;
                else if (min_idx == 2)
                    res.at<unsigned char>(i, j) = 170;
                else
                    res.at<unsigned char>(i, j) = 0;
            }
        }
    }

    double energy(Mat kep, int i, int j, int r=15) {
        int kernelRadius = r;
        int side = 2 * kernelRadius + 1;
        Mat kernel(side, side, CV_64FC1);
        for (int i = 0; i < kernel.rows; i++) {
            for (int j = 0; j < kernel.cols; j++) {
                kernel.at<double>(i, j) = 1.0;
            }
        }
        double sum = 0;
        for (int k = 0; k < 2 * kernelRadius + 1; k++) {
            for (int l = 0; l < 2 * kernelRadius + 1; l++) {
                if (i + k >= kep.rows)
                    i = 0;
                if (j + l >= kep.cols)
                    j = 0;
                if (i < 0)
                    i = kep.rows + (i);
                if (j < 0)
                    j = kep.cols + (j);
                sum += abs(kep.at<uchar>(i + k, j + l) * kernel.at<double>(k, l));
            }
        }
        return sum / ((2.0 * r + 1) * (2.0 * r + 1));
    }
    Mat matEnergy(Mat kep, int i, int j, int r= 15) {
        Mat result = kep.clone();

        for (int i = -r; i < kep.cols - (2 * r); i++) {
            for (int j = -r; j < kep.rows - (2 * r); j++) {
                result.at<double>(i + r, j + r) = energy(kep, i, j);
                //result.at<double>(i + W, j + W) = energia<double>(kep, i, j);
            }
        }
        return result;
    }

    void lawsFilter() {
        Mat laws_texture;
        cvtColor(cv::imread(INPUT), laws_texture, cv::COLOR_BGR2GRAY);
        Mat laws_input;
        cvtColor(cv::imread(TEXTURE), laws_texture, cv::COLOR_BGR2GRAY);

        list<Mat> inpConv = calcConv(laws_input);

        list<Mat> textEnergy;
        vector<pair<int,int>> kozeppontok;
        kozeppontok.emplace_back(49,63);
        kozeppontok.emplace_back(149,63);
        kozeppontok.emplace_back(49,191);
        kozeppontok.emplace_back(149,191);
        for (pair<int,int> pont: kozeppontok)
            for (const auto &item: inpConv){
                textEnergy.push_back(matEnergy(item, pont.first, pont.second));
            }

        list<Mat> textConv = calcConv(laws_texture);

        vector<Mat> vect(textConv.begin(), textConv.end());
        Mat out = laws(vect.at(0),
                       vect.at(1),
                       vect.at(2),
                       vect.at(3),
                       matEnergy(laws_input, 0, 0));
        imshow("laws",out);
    }
}
#endif //PROJECTNAME_LAWS_H
