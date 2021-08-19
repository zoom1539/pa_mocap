#pragma once

#ifdef API_EXPORTS
#if defined(_MSC_VER)
#define API __declspec(dllexport) 
#else
#define API __attribute__((visibility("default")))
#endif
#else
#if defined(_MSC_VER)
#define API __declspec(dllimport) 
#else
#define API 
#endif
#endif

#include "opencv2/opencv.hpp"

class API MoCap
{
public:
    explicit MoCap();
    ~MoCap();

    bool init(std::string &detector_wts_path_,
              const std::string &hmr_wts_path_);

    bool run(const cv::Mat &img_, std::vector<cv::Vec3f> &pose_);

private:
    MoCap(const MoCap &);
    const MoCap &operator=(const MoCap &);

    class Impl;
    Impl *_impl;
};
