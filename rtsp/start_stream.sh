#!/bin/bash
gst-launch-0.10 v4l2src device=${1} ! 'video/x-raw-yuv,width=640,height=480,framerate=30/1' ! \
    queue ! ffmpegcolorspace ! jpegenc ! \
    queue ! avimux name=mux ! \
    udpsink host=${2} port=${3}
