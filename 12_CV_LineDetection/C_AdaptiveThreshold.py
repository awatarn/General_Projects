#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Dec  5 20:25:17 2017

@author: vitreloy
"""

import cv2
import numpy as np
from matplotlib import pyplot as plt

# Load image
#img = cv2.imread('Clean5.JPG')
#img = cv2.imread('Black1.JPG')
img = cv2.imread('Dirty5.JPG')

plt.imshow(img)
plt.show()

img = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)

ret,th1 = cv2.threshold(img,127,255,cv2.THRESH_BINARY)

plt.imshow(th1)
plt.show()


#th2 = cv2.adaptiveThreshold(img,255,cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,11,2)
th2 = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, 5, 10)

plt.imshow(th2)
plt.show()

th3 = cv2.adaptiveThreshold(img,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,cv2.THRESH_BINARY,11,2)

plt.imshow(th3)
plt.show()

titles = ['Original Image', 'Global Thresholding (v = 127)',
            'Adaptive Mean Thresholding', 'Adaptive Gaussian Thresholding']
images = [img, th1, th2, th3]

for i in range(4):
    plt.subplot(2,2,i+1),plt.imshow(images[i],'gray')
    plt.title(titles[i])
    plt.xticks([]),plt.yticks([])
    
plt.show()