#ifndef __kinect_hpp__
#define __kinect_hpp__

#include <pthread.h>
#include <vector>
#include <libfreenect.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class KinectDevice : public Freenect::FreenectDevice {
  private:
    pthread_mutex_t depth_lock;
    pthread_mutex_t video_lock;
    std::vector<uint8_t> depth_buffer;
    std::vector<uint8_t> video_buffer;
    std::vector<uint16_t> gamma_buffer;
    bool new_depth_frame;
    bool new_video_frame;
    cv::Mat depthMat;
    cv::Mat videoMat;

    bool new_bgr_frame;
    bool new_meters_frame;
    cv::Mat raw2bgrMat;
    cv::Mat distanceMat;
    cv::Vec3b raw2bgr(uint16_t raw);
    double raw2meters(uint16_t raw);
  public:
    KinectDevice(freenect_context *_ctx, int _index);
    ~KinectDevice();
    // Do not call directly even in child
    void DepthCallback(void *data, uint32_t timestamp);
    // Do not call directly even in child
    void VideoCallback(void *data, uint32_t timestamp);
    bool getDepth(cv::Mat& output);
    bool getVideo(cv::Mat& output);
    bool getDepthAsBGR(cv::Mat& output);
    bool getDistanceMat(cv::Mat& output);
};

#endif
