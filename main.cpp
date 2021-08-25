#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <chrono>
#include "class_mocap.h"
#include "opencv2/opencv.hpp"

int main()
{
    //MoCap mocap;

    //
    char *detect_wts_path = "../lib/extra/detector/yolov5s.wts";
	char *hmr_wts_path = "../lib/extra/hmr/hmr.wts";
    
    bool is_init = init(detect_wts_path, 
                        hmr_wts_path);
    if(!is_init) 
    {
        std::cout << "init fail\n";
        return 0;
    }

    //
    
    cv::Mat img = cv::imread("../data/im1010.jpg");
    
    int iter = 1;
    double total_time = 0;

    for (int i = 0; i < iter; i++)
    {
		auto start = std::chrono::system_clock::now();

        // std::vector<cv::Vec3f> pose;
        float pose[72];
        bool is_run = run(img.data, img.cols, img.rows, pose, 72);
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
            for (int i = 0; i < 24 * 3; i++)
            {
                 std::cout << pose[i] << std::endl;
            }
        }
    }

    std::cout << "average: " << total_time / iter << " ms" << std::endl;

	release();

    std::cin.get();
    return 0;
}

