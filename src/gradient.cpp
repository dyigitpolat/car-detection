#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "functions.hpp"

using namespace std;
using namespace cv;

int main( int argc, char** argv)
{
    // Read images
    for( int i = 1; i < argc; i++)
    {
        Mat img = imread(argv[i], CV_LOAD_IMAGE_COLOR);
        Mat img_gray, gx, gy;

        cvtColor( img, img_gray, CV_BGR2GRAY);
        Sobel( img_gray, gx, CV_16S, 1,0,3,1,0,BORDER_DEFAULT);
        gx = (gx + 128)/2;
        Sobel( img_gray, gy, CV_16S, 0,1,3,1,0,BORDER_DEFAULT);
        gy = (gy + 128)/2;

        mysave( "gradients/GX_"+string(argv[i]), gx);
        mysave( "gradients/GY_"+string(argv[i]), gy);
    }
    
    return 0;
}