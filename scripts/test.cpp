// Script to test if openCV is working properly.
// g++ scripts/test.cpp -o scripts/output/test -I./opencv-install/include/opencv4 -L./opencv-install/lib -lopencv_world -Wl,-rpath,./opencv-install/lib
#include <opencv2/opencv.hpp>
#include <vector>

int main() {
    // Create a black image
    cv::Mat img(512, 512, CV_8UC3, cv::Scalar(0, 0, 0));

    // Check if image is created successfully
    if (img.empty()) {
        printf("Error: Image not created!");
        return -1;
    }

    // Define the vertices of the triangle
    cv::Point pt1(256, 100);
    cv::Point pt2(100, 400);
    cv::Point pt3(412, 400);

    // Define colors for each vertex (BGR)
    cv::Vec3b color1(0, 0, 255); // Red
    cv::Vec3b color2(0, 255, 0); // Green
    cv::Vec3b color3(255, 0, 0); // Blue

    // Get the bounding box of the triangle
    cv::Rect roi = cv::boundingRect(std::vector<cv::Point>{pt1, pt2, pt3});

    // Iterate over each pixel in the bounding box
    for (int y = roi.y; y < roi.y + roi.height; ++y) {
        for (int x = roi.x; x < roi.x + roi.width; ++x) {
            cv::Point p(x, y);

            // Calculate barycentric coordinates
            float detT = (float)((pt2.y - pt3.y) * (pt1.x - pt3.x) + (pt3.x - pt2.x) * (pt1.y - pt3.y));
            float w1 = ((float)((pt2.y - pt3.y) * (p.x - pt3.x) + (pt3.x - pt2.x) * (p.y - pt3.y))) / detT;
            float w2 = ((float)((pt3.y - pt1.y) * (p.x - pt3.x) + (pt1.x - pt3.x) * (p.y - pt3.y))) / detT;
            float w3 = 1.0f - w1 - w2;

            // Check if the pixel is inside the triangle
            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                // Interpolate the color
                cv::Vec3b final_color;
                final_color[0] = w1 * color1[0] + w2 * color2[0] + w3 * color3[0];
                final_color[1] = w1 * color1[1] + w2 * color2[1] + w3 * color3[1];
                final_color[2] = w1 * color1[2] + w2 * color2[2] + w3 * color3[2];
                img.at<cv::Vec3b>(y, x) = final_color;
            }
        }
    }

    // Create a window
    cv::namedWindow("Triangle Test", cv::WINDOW_AUTOSIZE);

    // Show the image in the window
    cv::imshow("Triangle Test", img);

    // Wait for a key press
    cv::waitKey(0);

    return 0;
}
