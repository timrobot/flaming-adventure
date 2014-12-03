#include "kinect.hpp"
#include <math.h>
#include <stdio.h>

using namespace cv;
using namespace std;

/* This file accesses the Kinect Device and gets its video and depth frames. If a depth frame is deteced, a new distance frame is created as well */

KinectDevice::KinectDevice(freenect_context *_ctx, int _index) :
  Freenect::FreenectDevice(_ctx, _index),
  depth_buffer(FREENECT_DEPTH_11BIT),
  video_buffer(FREENECT_VIDEO_RGB),
  gamma_buffer(2048),
  new_depth_frame(false),
  new_video_frame(false),
  depthMat(Size(640, 480), CV_16UC1),
  videoMat(Size(640, 480), CV_8UC3),
  new_bgr_frame(false),
  new_meters_frame(false),
  raw2bgrMat(Size(640, 480), CV_8UC3),
  distanceMat(Size(640, 480), sizeof(double)) {
  for (uint32_t i = 0; i < 2048; i++) {
    float v = i / 2048.0;
    v = pow(v, 3) * 6;
    gamma_buffer[i] = v * 6 * 256;
  }
  pthread_mutex_init(&depth_lock, NULL);
  pthread_mutex_init(&video_lock, NULL);
}

KinectDevice::~KinectDevice() {
  pthread_mutex_destroy(&depth_lock);
  pthread_mutex_destroy(&video_lock);
}

void KinectDevice::DepthCallback(void *data, uint32_t timestamp) {
  pthread_mutex_lock(&depth_lock);
  depthMat.data = (uint8_t *)(uint16_t *)data;
  new_depth_frame = true;
  new_bgr_frame = true;
  new_meters_frame = true;
  pthread_mutex_unlock(&depth_lock);
}

void KinectDevice::VideoCallback(void *data, uint32_t timestamp) {
  pthread_mutex_lock(&video_lock);
  videoMat.data = (uint8_t *)data;
  new_video_frame = true;
  pthread_mutex_unlock(&video_lock);
}

bool KinectDevice::getDepth(Mat& output) {
  pthread_mutex_lock(&depth_lock);
  if (new_depth_frame) {
    depthMat.copyTo(output);
    new_depth_frame = false;
    pthread_mutex_unlock(&depth_lock);
    return true;
  } else {
    depthMat.copyTo(output);
    pthread_mutex_unlock(&depth_lock);
    return false;
  }
}

bool KinectDevice::getVideo(Mat& output) {
  pthread_mutex_lock(&video_lock);
  if (new_video_frame) {
    cvtColor(videoMat, output, CV_RGB2BGR);
    new_video_frame = false;
    pthread_mutex_unlock(&video_lock);
    return true;
  } else {
    cvtColor(videoMat, output, CV_RGB2BGR);
    pthread_mutex_unlock(&video_lock);
    return false;
  }
}

uint16_t sigfun(double x) {
  return (uint16_t)(x * 150.0);
}

// converts depth into color-friendly image to display
bool KinectDevice::getDepthAsBGR(Mat& output) {
  if (new_depth_frame) {
    getDepth(depthMat);
  }
  if (new_bgr_frame) {
    for (int y = 0; y < depthMat.rows; y++)
      for (int x = 0; x < depthMat.cols; x++)
        raw2bgrMat.at<Vec3b>(y, x) = raw2bgr(sigfun(raw2meters(depthMat.at<uint16_t>(y, x))));
    raw2bgrMat.copyTo(output);
    new_bgr_frame = false;
    return true;
  } else {
    raw2bgrMat.copyTo(output);
    return false;
  }
}

bool KinectDevice::getDistanceMat(Mat &output) {
  if (new_depth_frame) {
    getDepth(depthMat);
  }
  if (new_meters_frame) {
    for (int y = 0; y < depthMat.rows; y++)
      for (int x = 0; x < depthMat.cols; x++)
        distanceMat.at<Vec3b>(y, x) = raw2meters(depthMat.at<uint16_t>(y, x));
    distanceMat.copyTo(output);
    new_meters_frame = false;
    return true;
  } else {
    distanceMat.copyTo(output);
    return false;
  }
}

Vec3b KinectDevice::raw2bgr(uint16_t raw) {
  Vec3b bgr;
  switch (raw / 410) {
    case 0:
      bgr[0] = raw * 255 / 410;
      break;
    case 1:
      bgr[0] = 255;
      raw -= 410;
      bgr[1] = raw * 255 / 410;
      break;
    case 2:
      bgr[1] = 255;
      raw -= 820;
      raw = 410 - raw;
      bgr[0] = raw * 255 / 410;
      break;
    case 3:
      bgr[1] = 255;
      raw -= 1230;
      bgr[2] = raw * 255 / 410;
      break;
    case 4:
      bgr[2] = 255;
      raw -= 1640;
      raw = 410 - raw;
      bgr[1] = raw * 255 / 410;
    default:
      bgr[0] = 255;
      bgr[1] = 255;
      bgr[2] = 255;
      break;
  }
  return bgr;
}

double KinectDevice::raw2meters (uint16_t raw) {
    return (0.1236 * tan(raw / 2842.5 + 1.1863));
}
