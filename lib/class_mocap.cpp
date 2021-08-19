#include "class_mocap.h"
#include "class_mocap_.h"

class MoCap::Impl
{
public:
    _MoCap _MoCap;
};

MoCap::MoCap() : _impl(new MoCap::Impl())
{
}

MoCap::~MoCap()
{
    delete _impl;
    _impl = NULL;
}

bool MoCap::init(std::string &detector_wts_path_,
              const std::string &hmr_wts_path_)
{
    return _impl->_MoCap.init(detector_wts_path_, 
                              hmr_wts_path_);
}

bool MoCap::run(const cv::Mat &img_, std::vector<cv::Vec3f> &pose_)
{
    return _impl->_MoCap.run(img_, pose_);
}


