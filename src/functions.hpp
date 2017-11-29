#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void mysave(const String &name, Mat m)
{
    vector<int> params;
    params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    params.push_back(5);
    try
    {
        imwrite(name+".png", m, params);
        cout << name+".png" << endl;
    }
    catch(runtime_error& ex)
    {
        printf("Exception converting image to PNG format: %s\n", ex.what());
    }

}

void myshow(const String &name, Mat m)
{
    cv::imshow(name, m);
    moveWindow(name, 150,150);
}

void showplot(const String &name, Mat xData, int xmin, int xmax, int ymin, int ymax)
{
    Mat plot_result;
    Mat norm;

    normalize(xData, norm, 0, ymax * 3 / 4, NORM_MINMAX, CV_64F);
    Ptr<plot::Plot2d> plot = plot::Plot2d::create(norm);
    plot->setMaxX(xmax);
    plot->setMinX(xmin);
    plot->setMaxY(ymax);
    plot->setMinY(ymin);
    plot->setInvertOrientation(true);
    plot->setShowGrid(false);
    plot->setPlotSize(xData.cols, xData.cols / 3);
    plot->setPlotLineWidth(2);
    plot->setPlotBackgroundColor( Scalar( 50, 50, 50 ) );
    plot->setPlotLineColor( Scalar( 255, 50, 50 ) );
    plot->setPlotAxisColor( Scalar( 50, 50, 200 ) );
    plot->render( plot_result );
    plot->setShowText(false);

    myshow( name, plot_result );
}

void peaks_n_pits( Mat x, 
                vector<double> &peaks_i,
                vector<double> &peaks_v,
                vector<double> &pits_i,
                vector<double> &pits_v)
{
    for( int i = 1; i < x.cols - 1; i++)
    {
        if( x.at<double>(i) >= x.at<double>(i-1) &&
            x.at<double>(i) > x.at<double>(i+1))
        {
            peaks_i.push_back(i);
            peaks_v.push_back(x.at<double>(i));
        }

        if( x.at<double>(i) <= x.at<double>(i-1) &&
            x.at<double>(i) < x.at<double>(i+1))
        {
            pits_i.push_back(i);
            pits_v.push_back(x.at<double>(i));
        }
    }
}

double peak_score( int peak_i, double peak_val, vector<double> &pits_i, Mat x)
{
    double score = 0;
    if( peak_i >= 0 && peak_i <= pits_i[0])
    {
        score = peak_val - x.at<double>(0);
    }

    for( int i = 0; i < pits_i.size() - 1; i++)
    {
        if( peak_i >= pits_i[i] && peak_i <= pits_i[i+1])
        {
            score = peak_val - x.at<double>(pits_i[i+1]);
        }
    }

    if( peak_i >= pits_i[pits_i.size() -1] && peak_i <= x.cols)
    {
        score = peak_val - x.at<double>(x.cols-1);
    }

    return score;
}

double calculate_width(int point, vector<double> &steps, int max)
{
    double width = 0;
    if( point >= 0 && point <= steps[0])
    {
        width = steps[0];
    }

    for( int i = 0; i < steps.size() - 1; i++)
    {
        if( point >= steps[i] && point <= steps[i+1])
        {
            width = steps[i+1] - steps[i];
        }
    }

    if( point >= steps[steps.size() -1] && point <= max)
    {
        width = max - steps[steps.size() -1];
    }

    return width;
}
