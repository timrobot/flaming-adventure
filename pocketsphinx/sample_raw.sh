#!/bin/bash
#gst-launch-0.10 -v alsasrc ! "audio/x-raw-int, endianness=1234, signed=true, width=32, height=32, rate=44100, channels=2" ! queue . ! filesink location="$1"
gst-launch-0.10 -v alsasrc ! "audio/x-raw-int, endianness=1234, signed=true, width=16, height=16, rate=16000, channels=1" ! queue . ! filesink location="$1"
