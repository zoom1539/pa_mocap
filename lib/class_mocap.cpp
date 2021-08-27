#include "class_mocap.h"
#include "class_mocap_.h"


// class MoCap::Impl
// {
// public:
//     _MoCap _MoCap;
// };

// MoCap::MoCap() : _impl(new MoCap::Impl())
// {
// }

// MoCap::~MoCap()
// {
//     delete _impl;
//     _impl = NULL;
// }

// bool MoCap::init(std::string &detector_wts_path_,
//               const std::string &hmr_wts_path_)
// {
//     return _impl->_MoCap.init(detector_wts_path_, 
//                               hmr_wts_path_);
// }

// bool MoCap::run(const cv::Mat &img_, std::vector<cv::Vec3f> &pose_)
// {
//     return _impl->_MoCap.run(img_, pose_);
// }


static _MoCap *_mocap;

bool init(char *detector_wts_path_, char *hmr_wts_path_, char *smpl_male_json_path_)
{
	_mocap = new _MoCap();

	std::string detector_wts_path = detector_wts_path_;
	std::string hmr_wts_path = hmr_wts_path_;
	std::string smpl_male_json_path = smpl_male_json_path_;

    return _mocap->init(detector_wts_path, hmr_wts_path, smpl_male_json_path);
}
   
bool run(unsigned char * data_, int width_, int height_, float *joints3d_, int joints3d_len_)
{
#if 1 // unity version
    cv::Mat img = cv::Mat(height_, width_, CV_8UC4, data_);
    cv::cvtColor(img, img, CV_RGBA2BGR);
    cv::flip(img, img, 0); // up to down
#else // c++ version
	cv::Mat img = cv::Mat(height_, width_, CV_8UC3, data_);
#endif

    std::vector<cv::Vec3f> joints3d;
    bool is_run = _mocap->run(img, joints3d);
    if(is_run)
    {
        for (int i = 0; i < joints3d_len_; i+=3)
        {
            joints3d_[i]     = joints3d[i / 3][0];
            joints3d_[i + 1] = -joints3d[i / 3][1]; // turn to stand-up
            joints3d_[i + 2] = -joints3d[i / 3][2];
        }
        return true;
    }
    else
    {
        return false;
    }
}

void release()
{
	if (_mocap)
	{
		delete _mocap;
		_mocap = nullptr;
	}
}


