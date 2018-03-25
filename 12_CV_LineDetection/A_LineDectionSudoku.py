#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Dec  5 11:41:00 2017
This illustrates a simple method for detecting lines in a figure.
It is based on Hough line transformation. 
See more details at 
https://docs.opencv.org/3.0-beta/doc/py_tutorials/py_imgproc/py_houghlines/py_houghlines.html
@author: A. Wisitsorasak

"""
import numpy as np
import cv2
from matplotlib import pyplot as plt

# Load image
img = cv2.imread('Clean5.JPG')
#img = cv2.imread('Black1.JPG')
#img = cv2.imread('Dirty5.JPG')

plt.imshow(img)
plt.show()

# Convert to grayscale
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

#plt.imshow(gray)
#plt.show()

# Apply Canny edge detection, return will be a binary image
edges = cv2.Canny(gray,4,100,apertureSize = 3)

plt.imshow(edges, cmap = 'gray', interpolation = 'bicubic')
plt.show()

# Apply Hough Line Transform, minimum lenght of line is 200 pixels
lines = cv2.HoughLines(edges,1,np.pi/180,150)

print(len(lines))

for i in range(len(lines)):
    for rho,theta in lines[i]:
        a = np.cos(theta)
        b = np.sin(theta)
        x0 = a*rho
        y0 = b*rho
        x1 = int(x0 + 1000*(-b))
        y1 = int(y0 + 1000*(a))
        x2 = int(x0 - 1000*(-b))
        y2 = int(y0 - 1000*(a))
        cv2.line(img,(x1,y1),(x2,y2),(0,0,255),2)

plt.imshow(img, cmap = 'gray', interpolation = 'bicubic')
plt.show()
cv2.imwrite('houghlines_xxx.jpg',img)

