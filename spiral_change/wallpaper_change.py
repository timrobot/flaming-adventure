#!/usr/bin/env python
import time, os, sys, cv2

dest = sys.argv[1]

imagenames = sys.argv[2:]
if len(imagenames) == 0:
  print "Error: no images"
  sys.exit()

print "Images: " + str(imagenames)

images = map(lambda s: cv2.imread(s), imagenames)

i = 0

while True:
  i = (i + 1) % len(images)
  image = images[i]
  cv2.imwrite(dest, image)
  time.sleep(0.05)
