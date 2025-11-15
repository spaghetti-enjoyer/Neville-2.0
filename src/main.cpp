#include <iostream>
#include <string>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include "neville.hpp"
#include <serial/serial.h>
#include <opencv2/opencv.hpp>

namespace
{
    constexpr uint8_t STOP_SPEED = 0x00;
    constexpr uint8_t DRIVE_SPEED = 0x10;

    int read_env_int(const char* name, int fallback)
    {
        if (const char* value = std::getenv(name))
        {
            try
            {
                return std::stoi(value);
            }
            catch (const std::exception&)
            {
                std::cerr << "Invalid value for " << name << ": '" << value
                          << "'. Using " << fallback << " instead." << std::endl;
            }
        }
        return fallback;
    }

    bool apply_drive_command(int key, Neville& neville)
    {
        if (key < 0 || key > 255)
        {
            return false;
        }

        const char normalized = static_cast<char>(
            std::tolower(static_cast<unsigned char>(key)));

        switch (normalized)
        {
            case 'c':
                neville.activate_motor(motor::LEFT, direction::FORWARD, STOP_SPEED);
                neville.activate_motor(motor::RIGHT, direction::FORWARD, STOP_SPEED);
                return true;
            case 'w':
                neville.activate_motor(motor::LEFT, direction::FORWARD, DRIVE_SPEED);
                neville.activate_motor(motor::RIGHT, direction::FORWARD, DRIVE_SPEED);
                return true;
            case 'a':
                neville.activate_motor(motor::LEFT, direction::FORWARD, STOP_SPEED);
                neville.activate_motor(motor::RIGHT, direction::FORWARD, DRIVE_SPEED);
                return true;
            case 's':
                neville.activate_motor(motor::LEFT, direction::REVERSE, DRIVE_SPEED);
                neville.activate_motor(motor::RIGHT, direction::REVERSE, DRIVE_SPEED);
                return true;
            case 'd':
                neville.activate_motor(motor::LEFT, direction::FORWARD, DRIVE_SPEED);
                neville.activate_motor(motor::RIGHT, direction::FORWARD, STOP_SPEED);
                return true;
            default:
                return false;
        }
    }

    bool run_camera_loop(Neville& neville, int cam_index)
    {
        cv::VideoCapture video(cam_index);
        if (!video.isOpened())
        {
            std::cerr << "Unable to open camera index " << cam_index << "." << std::endl;
            std::cerr << "Use NEVILLE_CAMERA_INDEX to select another device." << std::endl;
            return false;
        }

        const std::string window_name = "Neville camera (" + std::to_string(cam_index) + ")";
        cv::namedWindow(window_name);

        cv::Mat frame;
        while (true)
        {
            if (!video.read(frame) || frame.empty())
            {
                std::cerr << "Camera delivered an empty frame. Ensure the device is connected and not used elsewhere." << std::endl;
                continue;
            }

            cv::rotate(frame, frame, cv::ROTATE_90_COUNTERCLOCKWISE);
            cv::imshow(window_name, frame);

            int key = cv::waitKey(30);
            if (key < 0)
            {
                continue;
            }

            key = std::tolower(static_cast<unsigned char>(key));
            if (key == 27 || key == 'q')
            {
                apply_drive_command('c', neville);
                break;
            }

            apply_drive_command(key, neville);
        }

        cv::destroyWindow(window_name);
        return true;
    }
}


int main()
{
    const char* override_port = std::getenv("NEVILLE_SERIAL_PORT");
    const std::string port = override_port ? override_port : "/dev/ttyUSB0";
    const int cam_index = read_env_int("NEVILLE_CAMERA_INDEX", 0);

    try
    {
        Neville neville(port);
        neville.buzz();

        if (!run_camera_loop(neville, cam_index))
        {
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
    catch (serial::IOException& ex)
    {
        std::cerr << "Failed to open serial port '" << port << "': " << ex.what() << std::endl;

        if (ex.getErrorNumber() == EACCES)
        {
            std::cerr << "Permission denied indicates that your user is not allowed to access "
                      << port << "." << std::endl;
            std::cerr << "Add your user to the 'dialout' group and log out/in:" << std::endl;
            std::cerr << "  sudo usermod -aG dialout $USER" << std::endl;
            std::cerr << "You can also temporarily run: sudo chmod a+rw " << port << std::endl;
        }

        std::cerr << "Set NEVILLE_SERIAL_PORT to override the default device." << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Unhandled error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
}
