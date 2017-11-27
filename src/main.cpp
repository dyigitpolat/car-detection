#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "functions.hpp"

using namespace std;
using namespace cv;


int main( int argc, char** argv)
{
    // Read image
    Mat img = imread(argv[1] ,CV_LOAD_IMAGE_COLOR);
    Mat img_gray, gx;

    namedWindow( "window_name", CV_WINDOW_AUTOSIZE );
    cvtColor( img, img_gray, CV_BGR2GRAY);
    Sobel( img_gray, gx, CV_16S, 1,0,3,1,0,BORDER_DEFAULT);
    gx = (gx + 128)/2;
    imshow( "window_name", gx );
    mysave( "asdgf", gx);

    waitKey(0);

    return 0;
}