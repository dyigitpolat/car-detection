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
