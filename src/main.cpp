#include <iostream>
#include <opencv2/opencv.hpp>

int main() {

    cv::VideoCapture camera{0};

    if (!camera.isOpened()) {
        std::cerr << "Failed to open camera\n";
        return 1;
    }

    cv::Mat frame;

    std::cout << "Camera opened successfully\n";

    while (true) {
        if (!camera.read(frame)) {
            std::cerr << "Cannot read frame\n";
            break;
        }

        if (frame.empty()) {
            std::cerr << "Frame empty!\n";
            return 1;
        }

        cv::imshow("frame", frame);

        int key = cv::waitKey(1);

        if (key == 'q') {
            std::cout << "Quitting\n";
            break;
        }
    }

    cv::destroyAllWindows();

    return 0;
}