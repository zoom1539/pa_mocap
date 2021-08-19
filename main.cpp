#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <chrono>
#include "class_mocap.h"

int main()
{
    MoCap mocap;

    //
    std::string detect_wts_path = "../lib/extra/detector/yolov5s.wts";
    std::string hmr_wts_path = "../lib/extra/hmr/hmr.wts";
    
    bool is_init = mocap.init(detect_wts_path, 
                                hmr_wts_path);
    if(!is_init)
    {
        std::cout << "init fail\n";
        return 0;
    }

    //
    
    cv::Mat img = cv::imread("../data/im1010.jpg");
    
    int iter = 10;
    double total_time = 0;

    for (int i = 0; i < iter; i++)
    {
		auto start = std::chrono::system_clock::now();

        std::vector<cv::Vec3f> pose;
        bool is_run = mocap.run(img, pose);
        if(!is_run)
        {
            std::cout << "no people detected\n";
        }

		auto end = std::chrono::system_clock::now();
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " total ms" << std::endl;
        total_time += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        if (is_run)
        {
            std::cout << "-------------" << std::endl;
            for (int i = 0; i < pose.size(); i++)
            {
                 std::cout << pose[i] << std::endl;
            }
        }
    }

    std::cout << "average: " << total_time / iter << " ms" << std::endl;

    std::cin.get();
    return 0;
}

