//
// Created by papdawin on 2022. 11. 22..
//

#ifndef PROJECTNAME_BOGARKOVETO_H
#define PROJECTNAME_BOGARKOVETO_H


#define BUG "/Users/pdwn/Desktop/bug.bmp"
#define BUG0 "/Users/pdwn/Desktop/bug0.bmp"

namespace bogarKoveto{
    using namespace std;
    enum direction{
        NORTH=0,
        EAST=1,
        SOUTH=2,
        WEST=3
    };
    class Bug{
    public:
        pair<int,int> startingPosition;
        int x,y;
        bool right;
        int direction=0, pixelValue=0;
        Bug(Mat kep){
            pair<int,int> pos = getStart(kep);
            pos.first--;
            x=pos.first;
            y=pos.second;
            right=true;
            direction=EAST;
            startingPosition=pos;
        }
        virtual ~Bug() =default;
        pair<int,int> getStart(Mat kep){
            for (int kepRow = 0; kepRow < kep.rows; kepRow++)
                for (int kepCol = 0; kepCol < kep.cols; kepCol++)
                    if(kep.at<uchar>(kepRow,kepCol)==255)
                        return {kepRow,kepCol};
        }
        void moveForward(){
            switch (direction) {
                case NORTH:
                    x--;
                    break;
                case EAST:
                    y++;
                    break;
                case SOUTH:
                    x++;
                    break;
                case WEST:
                    y--;
                    break;
            }
        }
        void moveBackward(){
            switch (direction) {
                case NORTH:
                    x++;
                    break;
                case EAST:
                    y--;
                    break;
                case SOUTH:
                    x--;
                    break;
                case WEST:
                    y++;
                    break;
            }
        }
        void setDirection(){
            switch (direction) {
                case NORTH:
                    if(right)
                        direction=EAST;
                    else
                        direction=WEST;
                    break;
                case EAST:
                    if(right)
                        direction=SOUTH;
                    else
                        direction=NORTH;
                    break;
                case SOUTH:
                    if(right)
                        direction=WEST;
                    else
                        direction=EAST;
                    break;
                case WEST:
                    if(right)
                        direction=NORTH;
                    else
                        direction=SOUTH;
                    break;
            }
            cout<<direction;
        }
        bool isnotAtStart(){
            //cout<<"X: "<<x<<", Y: "<<y<<", direction: "<<direction<<", right:"<<right<<", pval: "<<pixelValue<<endl;
            if(!(startingPosition==pair<int,int>{x,y} && right))
                return true;
            return false;
        }
    };
    Mat simple(Mat kep){
        Mat out = Mat::zeros(kep.rows, kep.cols, CV_8UC1);
        Bug* bug = new Bug(kep);
        pair<int,int> a = bug->startingPosition;
        bug->right=true;
        bug->direction=EAST;
        do{
            bug->moveForward();
            bug->pixelValue=kep.at<uchar>(bug->x,bug->y);
            if(bug->pixelValue==255){
                bug->right=false;
                bug->setDirection();
            }else{
                bug->right=true;
                bug->setDirection();
            }
            bug->pixelValue=kep.at<uchar>(bug->x,bug->y);
            out.at<uchar>(bug->x,bug->y)=255;
        }while(bug->isnotAtStart());
        delete bug;
        return out;
    }
    Mat backtrack(Mat kep){
        Mat out = Mat::zeros(kep.rows, kep.cols, CV_8UC1);
        Bug* bug = new Bug(kep);
        bug->right=true;
        bug->direction=EAST;
        do{
            bug->pixelValue=kep.at<uchar>(bug->x,bug->y);
            if(bug->pixelValue==255){
                bug->moveBackward();
                bug->right=false;
                bug->setDirection();
                bug->moveForward();
            }else{
                bug->right=true;
                bug->setDirection();
                bug->moveForward();
            }
            bug->pixelValue=kep.at<uchar>(bug->x,bug->y);
            out.at<uchar>(bug->x,bug->y)=255;
        }while(bug->isnotAtStart());
        delete bug;
        return out;
    }
    void simpleBogar(){
        Mat gsrc;
        cvtColor(cv::imread(BUG0), gsrc, cv::COLOR_BGR2GRAY);
        imshow("original", gsrc);
        Mat contour = simple(gsrc);
        imshow("contour",contour);
    }
    void backtrackBogar(){
        Mat gsrc;
        cvtColor(cv::imread(BUG0), gsrc, cv::COLOR_BGR2GRAY);
        imshow("original", gsrc);
        Mat contour = backtrack(gsrc);
        imshow("contour",contour);
    }
}

#endif //PROJECTNAME_BOGARKOVETO_H
