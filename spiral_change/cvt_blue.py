#!/usr/bin/env python
import os, sys
import cv2

imagenames = sys.argv[1:]

print imagenames

images = map(lambda s: cv2.imread(s), imagenames)

def cvt_blue(img):
  cols = len(img[0])
  rows = len(img)
  for x in range(cols):
    for y in range(rows):
      img[y][x][0] = max(img[y][x])
      img[y][x][1] = 0
      img[y][x][2] = 0
  return img

blue_images = map(cvt_blue, images)

newnames = map(lambda s: "mod_" + s, imagenames)
for i in range(len(newnames)):
  cv2.imwrite(newnames[i], blue_images[i])
