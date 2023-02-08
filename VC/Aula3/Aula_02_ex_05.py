from cv2 import CV_64F
from matplotlib import pyplot as plt
import numpy as np
import cv2
import sys

# Read the image
image_original = cv2.imread( sys.argv[1], cv2.IMREAD_UNCHANGED );

image_final = image_original.copy()
rows,cols,_ = image_original.shape
minVal, maxVal,_,_ = cv2.minMaxLoc(image_original)
print("min: "+ str(minVal))
print("max: "+str(maxVal))

for x in range(rows):
    for y in range(cols):
        image_final[x,y] = ((image_original[x,y] - minVal) / (maxVal-minVal)) * 255

# Create a vsiualization window (optional)
# CV_WINDOW_AUTOSIZE : window size will depend on image size
cv2.namedWindow( "Display window", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Display window", image_original )
cv2.namedWindow( "Display window 2", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Display window 2", image_final )


# Size
histSize = 256	 # from 0 to 255
# Intensity Range
histRange = [0, 256]

# Compute the histogram
hist_item = cv2.calcHist([image_original], [0], None, [histSize], histRange)

##########################################
# Drawing with openCV
# Create an image to display the histogram
histImageWidth = 512
histImageHeight = 512
color = (125)
histImage = np.zeros((histImageWidth,histImageHeight,1), np.uint8)

# Width of each histogram bar
binWidth = int (np.ceil(histImageWidth*1.0 / histSize))

# Normalize values to [0, histImageHeight]
cv2.normalize(hist_item, hist_item, 0, histImageHeight, cv2.NORM_MINMAX)

# Draw the bars of the nomrmalized histogram
for i in range (histSize):
	cv2.rectangle(histImage,  ( i * binWidth, 0 ), ( ( i + 1 ) * binWidth, int(hist_item[i]) ), (125), -1)

# ATTENTION : Y coordinate upside down
histImage = np.flipud(histImage)

cv2.imshow('colorhist 1', histImage)





# Size
histSize = 256	 # from 0 to 255
# Intensity Range
histRange = [0, 256]

# Compute the histogram
hist_item = cv2.calcHist([image_final], [0], None, [histSize], histRange)

##########################################
# Drawing with openCV
# Create an image to display the histogram
histImageWidth = 512
histImageHeight = 512
color = (125)
histImage = np.zeros((histImageWidth,histImageHeight,1), np.uint8)

# Width of each histogram bar
binWidth = int (np.ceil(histImageWidth*1.0 / histSize))

# Normalize values to [0, histImageHeight]
cv2.normalize(hist_item, hist_item, 0, histImageHeight, cv2.NORM_MINMAX)

# Draw the bars of the nomrmalized histogram
for i in range (histSize):
	cv2.rectangle(histImage,  ( i * binWidth, 0 ), ( ( i + 1 ) * binWidth, int(hist_item[i]) ), (125), -1)

# ATTENTION : Y coordinate upside down
histImage = np.flipud(histImage)

cv2.imshow('colorhist 2', histImage)
cv2.waitKey(0)
cv2.waitKey( 0 );
cv2.destroyWindow( "Display window" )
cv2.destroyWindow( "Display window 2" )


#so da para as imagens lena e ireland-06-02