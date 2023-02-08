# Aula_03_ex_02.py
#
# Mean Filter
#
# Paulo Dias - 09/2021

#import
import sys
import numpy as np
import cv2

def printImageFeatures(image):
	# Image characteristics
	if len(image.shape) == 2:
		height, width = image.shape
		nchannels = 1;
	else:
		height, width, nchannels = image.shape

	# print some features
	print("Image Height: %d" % height)
	print("Image Width: %d" % width)
	print("Image channels: %d" % nchannels)
	print("Number of elements : %d" % image.size)

# Read the image from argv
image = cv2.imread( sys.argv[1] , cv2.IMREAD_GRAYSCALE );
#image = cv2.imread( "./lena.jpg", cv2.IMREAD_GRAYSCALE );

if  np.shape(image) == ():
	# Failed Reading
	print("Image file could not be open!")
	exit(-1)

printImageFeatures(image)

cv2.imshow('Orginal', image)

# Median filter 3 
imageAFilter3 = cv2.GaussianBlur( image, (3,3), cv2.BORDER_DEFAULT) #n impares
cv2.namedWindow( "Median Filter 3 ", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Median Filter 3 ", imageAFilter3 )

# Median filter 5
imageAFilter5 = cv2.GaussianBlur( image, (5,5), cv2.BORDER_DEFAULT) #n impares
cv2.namedWindow( "Median Filter 5 ", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Median Filter 5 ", imageAFilter5 )

# Median filter 7 
imageAFilter7 = cv2.GaussianBlur( image, (7,7), cv2.BORDER_DEFAULT) #n impares
cv2.namedWindow( "Median Filter 7 ", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Median Filter 7 ", imageAFilter7 )


cv2.waitKey(0)


