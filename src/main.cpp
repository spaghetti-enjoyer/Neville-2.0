#include <iostream>
#include "neville.hpp"
#include <opencv2/opencv.hpp>


int main()
{
    const std::string port = "/dev/cu.usbserial-10";

    Neville neville(port);
    neville.buzz();

    int cam_index = 0;
    cv::Mat frame;
    cv::VideoCapture video(cam_index);

    while (true)
    {
        int key = cv::waitKey(30);
        // std::cout << key << std::endl;
        if (key == 27) break; // esc pressed

        switch(key)
        {
            case 'c':
                neville.activate_motor(motor::LEFT, direction::FORWARD, 0x00);
                neville.activate_motor(motor::RIGHT, direction::FORWARD, 0x00);
                break;
            case 'w':
                neville.activate_motor(motor::LEFT, direction::FORWARD, 0x10);
                neville.activate_motor(motor::RIGHT, direction::FORWARD, 0x10);
                break;
            case 'a':
                neville.activate_motor(motor::LEFT, direction::FORWARD, 0x00);
                neville.activate_motor(motor::RIGHT, direction::FORWARD, 0x10);
                break;
            case 's':
                neville.activate_motor(motor::LEFT, direction::REVERSE, 0x10);
                neville.activate_motor(motor::RIGHT, direction::REVERSE, 0x10);
                break;
            case 'd':
                neville.activate_motor(motor::LEFT, direction::FORWARD, 0x10);
                neville.activate_motor(motor::RIGHT, direction::FORWARD, 0x00);
                break;
            default:
                // std::cout << "unknown key: " << key << std::endl;
        }

        video >> frame;
        // cv::rotate(frame, frame, cv::ROTATE_90_COUNTERCLOCKWISE);
        cv::imshow("camera " + std::to_string(cam_index), frame);
    }
}
