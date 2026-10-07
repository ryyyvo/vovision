#include <chrono>
#include <format>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

int main()
{
    cv::VideoCapture camera{0, cv::CAP_V4L2};

    if (!camera.isOpened()) {
        std::cerr << "Failed to open camera\n";
        return 1;
    }

    camera.set(
        cv::CAP_PROP_FOURCC,
        cv::VideoWriter::fourcc('M', 'J', 'P', 'G')
    );

    camera.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    camera.set(cv::CAP_PROP_FPS, 60);

    cv::Mat frame;
    cv::Mat gray_frame; 

    std::cout << "Camera opened successfully\n";

    auto start_time = std::chrono::steady_clock::now();
    int frame_count{0};

    double fps{0.0};
    std::string fps_text{"FPS: 0.0"};

    while (true) {
        if (!camera.read(frame)) {
            std::cerr << "Cannot read frame\n";
            break;
        }

        if (frame.empty()) {
            std::cerr << "Frame empty!\n";
            break;
        }

        cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);

        ++frame_count;

        auto current_time = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed =
            current_time - start_time;

        if (elapsed >= std::chrono::seconds{1}) {
            fps = frame_count / elapsed.count();

            fps_text = std::format("{:.1f}", fps);

            std::cout << fps_text << "\n";

            frame_count = 0;
            start_time = current_time;
        }

        cv::putText(
            gray_frame,
            fps_text,
            cv::Point{20, 40},
            cv::FONT_HERSHEY_SIMPLEX,
            1.0,
            cv::Scalar{255},
            2
        );

        cv::imshow("frame", gray_frame);

        int key = cv::waitKey(1);

        if (key == 'q') {
            std::cout << "Quitting\n";
            break;
        }
    }

    cv::destroyAllWindows();

    return 0;
}