# Hack-E-Able

This is a C++ and Opencv2 based project


An innovative solution to understand impaired hearing and vocally impaired people.
The Objective of Hack-e-abled is to enable deaf and mute to communicate effectively and independently.
This system reads hand gesture using video input and Generates text as output.


ALGORITHM
Capture the image through camera and converts RGB source image to Grayscale image.
Set ROI (region of interest) using cvSetImageROI function.
Smooth the grayscale image using cvSmooth function.
Threshold the smoothed image by using cvThreshold function.
At last we find contours using contour detection, convex hull of palm region, convexity defects.
