#include <iostream>
#include <opencv2/opencv.hpp>
#include "transformations.h"
#include "convolutions.h"
#include "wallisfilter.h"
#include "nonlinear.h"
#include "morphology.h"
#include "bogarkoveto.h"
#include "laws.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    //HistogramTransformations::hisztogramSzethuzas();
    //HistogramTransformations::hisztogramKiegyenlites();
    //Convolutions::laplaceFilter();
    //Convolutions::sobelFilter();
    //WallisFilter::wallisFilter();
    //nonLinearFilter::nonLinearFilter();
    //Morphology::run();
    //bogarKoveto::simpleBogar();
    //bogarKoveto::backtrackBogar();
    //laws::lawsFilter();
    waitKey(0);
    return 0;
}

