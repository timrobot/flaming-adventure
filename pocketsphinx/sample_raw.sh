#!/bin/bash
gst-launch-0.10 -v alsasrc ! "audio/x-raw-int, endianness=1234, signed=true, width=16, height=${1}, rate=16000, channels=1" ! queue . ! filesink location="rawsample${1}.raw"
