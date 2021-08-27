#pragma once

// std
#include <opencv2/opencv.hpp>
#include "class_detector.h"
#include "class_hmr.h"

class _MoCap
{
public:
    _MoCap();
    ~_MoCap();

public:
    bool init(std::string &detector_wts_path_,
              const std::string &hmr_wts_path_,
              std::string &smpl_male_json_path_);

    bool run(const cv::Mat &img_, std::vector<cv::Vec3f> &joints3d_);
    
private:
    Detector _detector;
    HMR _hmr;
};
