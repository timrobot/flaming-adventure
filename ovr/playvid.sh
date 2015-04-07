#!/bin/bash
#gst-launch-0.10 v4l2src device=/dev/video0 ! videorate ! "video/x-raw-yuv,width=640,height=480,framerate=60/1" ! xvimagesink
gst-launch-1.0 -e v4l2src device=/dev/video0 ! \
    videorate ! \
    video/x-raw,width=\(int\)640,height=\(int\)480,framerate=30/1 ! \
    xvimagesink
#gst-launch-1.0 -e v4l2src device=/dev/video0 ! video/x-raw,format=(string)YUV2,width=640,height=480 ! xvimagesink
