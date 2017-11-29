#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/plot.hpp>
#include <vector>
#include <string> 
#include "functions.hpp"

using namespace std;
using namespace cv;


int main( int argc, char** argv)
{
    for( int f = 1; f < argc; f++)
    {
        Mat img = imread(argv[f], CV_LOAD_IMAGE_COLOR);
        Mat img_gray, gauss, gx, gy;

        cvtColor( img, img_gray, CV_BGR2GRAY);
        int gw = img.cols / 15;
        gw = (gw % 2 ? gw : gw + 1);
        GaussianBlur( img_gray, gauss, Size( gw, gw ), 0, 0 );

        Sobel( gauss, gx, CV_64F, 1,0,3,3,0,BORDER_DEFAULT);
        Sobel( gauss, gy, CV_64F, 0,1,3,3,0,BORDER_DEFAULT);

        Mat xData, yData, loc;
        xData.create(1, gx.cols, CV_64F);
        yData.create(1, gx.cols, CV_64F);
        loc.create(1, gx.cols, CV_64F);

        double maxx = 0;
        for( int i = 0; i < gx.cols*gx.rows; i++)
        {
            if( gx.at<double>(i) > maxx) maxx = gx.at<double>(i);
        }

        double maxy = 0;
        for( int i = 0; i < gy.cols*gy.rows; i++)
        {
            if( gy.at<double>(i) > maxy) maxy = gy.at<double>(i);
        }

        double threshx = maxx * 4 / 5;
        double threshy = maxy * 4 / 5;
        for( int i = 0; i < gx.cols; i++)
        {
            xData.at<double>(i) = 0;
            yData.at<double>(i) = 0;
            loc.at<double>(i) = 0;
            for( int j = 0; j < gx.rows; j++)
            {
                if(abs(gy.col(i).at<double>(j)) > threshy)
                {
                    xData.at<double>(i) += abs(gy.col(i).at<double>(j));
                    loc.at<double>(i) += abs(gy.col(i).at<double>(j)) * j;
                }

                if(abs(gx.col(i).at<double>(j)) > threshx)
                {
                    yData.at<double>(i) += abs(gx.col(i).at<double>(j));
                }
            }
            loc.at<double>(i) /= xData.at<double>(i);
        }

        int gauss_window = img.cols / 3;
        gauss_window = (gauss_window % 2 ? gauss_window : gauss_window + 1); 
        GaussianBlur( xData, xData, Size(gauss_window, 1 ), 0, 0 );
        GaussianBlur( yData, yData, Size(gauss_window, 1 ), 0, 0 );

        vector<double> xpeaks_i, xpeaks_v, xpits_i, xpits_v;
        peaks_n_pits(xData, xpeaks_i, xpeaks_v, xpits_i, xpits_v);

        Mat normx, normy, car;
        normalize(xData, normx, 0, 256, NORM_MINMAX, CV_64F);
        normalize(yData, normy, 0, 256, NORM_MINMAX, CV_64F);
        car = normx-normy;

        vector<double> cpeaks_i, cpeaks_v, cpits_i, cpits_v;
        peaks_n_pits( car, cpeaks_i, cpeaks_v, cpits_i, cpits_v);
        //showplot("car", car, 0, yData.cols, 0, 256);

        for( int i = 0; i < cpeaks_i.size(); i++)
        {
            if( peak_score(cpeaks_i[i], cpeaks_i[i], cpits_i, car) < img.rows / 5)
            {
                continue;
            }

            double width = calculate_width(cpeaks_i[i], cpits_i, img.cols) * 3 / 4;
            double height = width * 3 / 4;


            rectangle(img, 
                    Point(cpeaks_i[i] - width / 2, loc.at<double>((int)cpeaks_i[i]) - height / 2), 
                    Point(cpeaks_i[i] + width / 2, loc.at<double>((int)cpeaks_i[i]) + height / 2), 
                    Scalar( (21 + (i*23)) % 255,
                            (83 + (i*59)) % 255,
                            (137 + (i*83)) % 255
                    )
                , 2);
        }
        //myshow( "new", img );
        

        
        gx = (gx + 128)/2;
        gy = (gy + 128)/2;
        mysave( "results/gx"+to_string(f), gx);
        mysave( "results/gy"+to_string(f), gy);
        mysave( "results/FIN_"+to_string(f), img);
        saveplot( "results/HIST_VERT_"+to_string(f), normx, 0, yData.cols, 0, 256);
        saveplot( "results/HIST_HORZ_"+to_string(f), normy, 0, yData.cols, 0, 256);
        saveplot( "results/HIST_CAR_"+to_string(f), car, 0, yData.cols, 0, 256);

    }

    return 0;
}