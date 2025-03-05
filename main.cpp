#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <NumCpp.hpp>

#define ROI_WIDTH_LOWER_BOUND 0
#define ROI_WIDTH_UPPER_BOUND 637
#define ROI_HEIGHT_LOWER_BOUND 400
#define ROI_HEIGHT_UPPER_BOUND 480

void find_roi(cv::Mat *frame, int *roi_x, int *roi_y, cv::Mat &out_frame, float &out)
{
    // std::cout << *roi_x << " | " << *roi_y << "\n";
    int right_lane_x = ROI_WIDTH_UPPER_BOUND;
    int left_lane_x = ROI_WIDTH_LOWER_BOUND;
    int lane_width_from_right = 0;
    int lane_width_from_left = 0;
    float half_between_lanes = 0;

    bool is_right_lane_d = false;
    bool is_left_lane_d = false;
    bool is_stepped = false;

    if(frame->at<uchar>(*roi_x, *roi_y) == 255)
    {
        is_stepped = true;
        std::cout << "Da phat hien xe\n";
    }

    for(int i=*roi_x; i<=ROI_WIDTH_UPPER_BOUND - 1; i++)
    {
        if (is_stepped && frame->at<uchar>(*roi_y, i) != 255) {
            lane_width_from_right = i - *roi_x;
            break;
        }
        
        if (!is_stepped && frame->at<uchar>(*roi_y, i) == 255) {
            is_right_lane_d = true;
            right_lane_x = i;
            break;
        }
    }


    for(int i=*roi_x; i<=ROI_WIDTH_LOWER_BOUND - 1; i++)
    {
        if (is_stepped && frame->at<uchar>(*roi_y, i) != 255) {
            lane_width_from_left = *roi_x - i;
            break;
        }
        
        if (!is_stepped && frame->at<uchar>(*roi_y, i) == 255) {
            is_left_lane_d = true;
            left_lane_x = i;
            break;
        }
    }

    if (is_stepped)
    {
        if (lane_width_from_right >= lane_width_from_left)
        {
            is_right_lane_d = true;
            right_lane_x = *roi_x;
        }
        else 
        {
            is_left_lane_d = true;
            left_lane_x = *roi_x;
        }
    }
    
    if(is_left_lane_d && !is_right_lane_d)
    {
        right_lane_x += left_lane_x;
    }

    if(!is_left_lane_d && is_right_lane_d)
    {
        left_lane_x = -(ROI_WIDTH_UPPER_BOUND - right_lane_x);
    }

    half_between_lanes = (int)(right_lane_x - left_lane_x)/2;
    float roi_lane_center = left_lane_x+half_between_lanes;

    out = roi_lane_center;
    out_frame = *frame;

}

void find_roi_center(int &x, int &y)
{
    int width = ROI_WIDTH_UPPER_BOUND - ROI_WIDTH_LOWER_BOUND;
    int height = ROI_HEIGHT_UPPER_BOUND - ROI_HEIGHT_LOWER_BOUND;
    x = (int)(width/2);
    y = (int)(height/2);
}

cv::Mat getROI(const cv::Mat &mask, int x1, int x2, int y1, int y2) {
    cv::Rect roi_rect(x1, y1, x2 - x1, y2 - y1);
    return mask(roi_rect).clone();
}

int main()
{
    cv::VideoCapture cap(0);
    cv::Mat frame, gray, gaus, hsv, out_frame, mask;
    int roi_x, roi_y;
    float out;

    while(1)
    {
        cap.read(frame);

        cv::cvtColor(frame, gray, cv::COLOR_BGR2RGB);
        cv::GaussianBlur(gray, gaus, cv::Size(5, 5), 0);
        cv::cvtColor(gaus, hsv, cv::COLOR_BGR2HSV);


        cv::Scalar lower_white(0, 0, 200);
        cv::Scalar upper_white(180, 50, 255);

        cv::inRange(hsv, lower_white, upper_white, mask);

        cv::Mat roi = getROI(mask, ROI_WIDTH_LOWER_BOUND, ROI_WIDTH_UPPER_BOUND, 
                          ROI_HEIGHT_LOWER_BOUND, ROI_HEIGHT_UPPER_BOUND);
        find_roi_center(roi_x, roi_y);
        find_roi(&mask, &roi_x, &roi_y, out_frame, out);
        std::cout << out << "\n";

        cv::imshow("Frame", frame);
        if(cv::waitKey(1) == 'q') break;
    }
    cap.release();
}

