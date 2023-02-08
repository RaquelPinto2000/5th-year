from cv2 import CV_64F, equalizeHist
from matplotlib import pyplot as plt
import numpy as np
import cv2
import sys

# Read the image
image_original = cv2.imread( sys.argv[1], cv2.IMREAD_UNCHANGED );


rows,cols,_ = image_original.shape
b,g,r = cv2.split(image_original)
image_gray = cv2.cvtColor(image_original, cv2.COLOR_RGB2GRAY) 
# Create a vsiualization window (optional)
# CV_WINDOW_AUTOSIZE : window size will depend on image size
cv2.namedWindow( "Display window", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Display window", image_original )
cv2.namedWindow( "Display window 2", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Display window 2", image_gray )


# Size
histSize = 256	 # from 0 to 255
# Intensity Range
histRange = [0, 256]

# Compute the histogram
hist_item = cv2.calcHist([image_gray], [0], None, [histSize], histRange)

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

cv2.imshow('colorhist image_gray', histImage)

##########################################
# Drawing with openCV
# Create an image to display the histogram
# Size
color = ('b','g','r')
for i,col in enumerate(color):
    histr = cv2.calcHist([image_original],[i],None,[256],[0,256])
    plt.plot(histr,color = col)
    plt.xlim([0,256])
plt.show()
cv2.waitKey(0)

cv2.destroyWindow( "Display window" )
cv2.destroyWindow( "Display window 2" )