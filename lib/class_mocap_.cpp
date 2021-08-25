#include "class_mocap_.h"

// Checks if a matrix is a valid rotation matrix.
static bool isRotationMatrix(cv::Mat &R)
{
	cv::Mat Rt;
	cv::transpose(R, Rt);
	cv::Mat shouldBeIdentity = Rt * R;
	cv::Mat I = cv::Mat::eye(3,3, shouldBeIdentity.type());
    
    return  cv::norm(I, shouldBeIdentity) < 1e-6;
    
}

// Calculates rotation matrix to euler angles
// The result is the same as MATLAB except the order
// of the euler angles ( x and z are swapped ).
static cv::Vec3f rotationMatrixToEulerAngles(cv::Mat &R) // IMPORTANT: the type of the element in R should be float
{
    assert(isRotationMatrix(R));
    
    float sy = sqrt(R.at<float>(0,0) * R.at<float>(0,0) +  R.at<float>(1,0) * R.at<float>(1,0) );

    bool singular = sy < 1e-6; // If

    float x, y, z;
    if (!singular)
    {
        x = atan2(R.at<float>(2,1) , R.at<float>(2,2));
        y = atan2(-R.at<float>(2,0), sy);
        z = atan2(R.at<float>(1,0), R.at<float>(0,0));
    }
    else
    {
        x = atan2(-R.at<float>(1,2), R.at<float>(1,1));
        y = atan2(-R.at<float>(2,0), sy);
        z = 0;
    }
    return cv::Vec3f(x, y, z);
}


_MoCap::_MoCap() {}
_MoCap::~_MoCap() 
{
    
}

bool _MoCap::init(std::string &detector_wts_path_,
                  const std::string &hmr_wts_path_)
{
    //
    std::string detector_engine_path = detector_wts_path_.substr(0,detector_wts_path_.rfind(".")) + ".engine";
	std::ifstream detector_fin(detector_engine_path.c_str());
	if(!detector_fin.good())
	{
        int class_num = 80;
		_detector.serialize(detector_wts_path_, detector_engine_path, class_num);
	}
    bool is_detector = _detector.init(detector_engine_path);

    //
    std::string hmr_engine_path = hmr_wts_path_.substr(0,hmr_wts_path_.rfind(".")) + ".engine";
	std::ifstream hmr_fin(hmr_engine_path.c_str());
	if(!hmr_fin.good())
	{
		_hmr.serialize(hmr_wts_path_, hmr_engine_path);
	}
    bool is_hmr = _hmr.init(hmr_engine_path);

    //
	return is_detector & is_hmr;
}

bool _MoCap::run(const cv::Mat &img_, std::vector<cv::Vec3f> &pose_)
{
    std::vector<cv::Mat> imgs;
    imgs.push_back(img_);

    std::vector<std::vector<Detection> > vec_detections;
    _detector.detect(imgs, vec_detections);

    if (0 == vec_detections.size())
    {
        return false;
    }

    bool has_person = false;
    Detection max_area_detection;
    max_area_detection.rect = cv::Rect(0, 0, 0, 0);
    for (int i = 0; i < vec_detections[0].size(); i++)
    {
        if (vec_detections[0][i].class_id == 0)
        {
            has_person = true;
            if (vec_detections[0][i].rect.area() > max_area_detection.rect.area())
            {
               max_area_detection = vec_detections[0][i]; 
            }
        }
    }

    if (has_person)
    {
        std::vector<cv::Mat> imgs;
        imgs.push_back(img_(max_area_detection.rect));

        std::vector<std::vector<cv::Vec3f> > poses;
        std::vector<std::vector<float> > shapes;
        bool is_run = _hmr.run(imgs, poses, shapes);
        if (!is_run)
        {
            return false;
        }

        for (int i = 0; i < poses[0].size(); i++)
        {
            cv::Mat rotmat;
            cv::Rodrigues(poses[0][i], rotmat);
            cv::Vec3f euler = rotationMatrixToEulerAngles(rotmat);
            pose_.push_back(euler);
        }

        return true;
    }
    else
    {
        return false;
    }
}

