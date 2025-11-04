#include <iostream>
#include "neville.hpp"
#include <opencv2/opencv.hpp>

// cv::Mat capture_frame(cv::VideoCapture capture)
// {
//     cv::Mat frame;
//     capture >> frame;
//     cv::rotate(frame, frame, cv::ROTATE_90_COUNTERCLOCKWISE);
//     return frame;
// }

int main()
{
    const std::string port = "/dev/cu.usbserial-10";

    Neville neville(port);
    neville.buzz();

    int cam_index = 0;
    cv::Mat frame;
    cv::VideoCapture video(cam_index);

    video >> frame;
    cv::imshow("snapshot", frame);
    cv::waitKey(0);
}
