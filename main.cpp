#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <NumCpp.hpp>

cv::Mat find_roi(cv::Mat *frame, float *roi_x, float *roi_y)
{
    
}

int main()
{
    cv::VideoCapture cap(0);
    cv::Mat frame, gray, gaus, hsv;
    // nc::NdArray<int>

    while(1)
    {
        cap.read(frame);

        cv::cvtColor(frame, gray, cv::COLOR_BGR2RGB);
        cv::GaussianBlur(gray, gaus, cv::Size(5, 5), 0);
        cv::cvtColor(gaus, hsv, cv::COLOR_BGR2HSV);

        cv::imshow("Frame", hsv);
        if(cv::waitKey(1) == 'q') break;
    }
    cap.release();
}

