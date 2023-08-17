#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>

void captureVideo(const std::string& pipeline)
{
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    if (!cap.isOpened())
    {
        std::cerr << "Failed to open pipeline: " << pipeline << std::endl;
        return;
    }

    cv::Mat frame;
    std::string windowName = "Camera " + pipeline;
    cv::namedWindow(windowName, cv::WINDOW_NORMAL);

    while (true)
    {
        if (!cap.read(frame))
            break;

        cv::imshow(windowName, frame);
        if(cv::waitKey(1) == 'a')
        {
          cv::imwrite("captured_image.jpg",frame);
          std::cout <<"Picture taken..." << std::endl;
          cv::imshow("Captured image",frame);
        }
        // Exit the loop if 'q' is pressed
        if (cv::waitKey(1) == 'q')
            break;
            
    }

    cap.release();
    cv::destroyWindow(windowName);
}

int main()
{
    // Define the gstreamer pipeline parameters
    int sensor_id = 0;
    int capture_width = 1280;
    int capture_height = 720;
    int display_width = 960;  // Updated display width
    int display_height = 540;  // Updated display height
    int framerate = 30;
    int flip_method = 2;

    // Construct the gstreamer pipelines for the two cameras

    std::string pipeline1 = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=1280, height=720, format=NV12, framerate=30/1 ! nvvidconv flip-method=2 ! video/x-raw, width=640, height=480, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";


    std::string pipeline2 = "nvarguscamerasrc sensor-id=1 ! video/x-raw(memory:NVMM), width=1280, height=720, format=NV12, framerate=30/1 ! nvvidconv flip-method=2 ! video/x-raw, width=640, height=480, format=BGRx ! videoconvert ! video/x-raw, format=BGR ! appsink";
    

    // Start two threads to capture video from the two cameras
    std::thread t1(captureVideo, pipeline1);
    std::thread t2(captureVideo, pipeline2);

    // Wait for the threads to finish
    t1.join();
    t2.join();

    return 0;
}


