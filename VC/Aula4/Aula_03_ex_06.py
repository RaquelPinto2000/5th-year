# Aula_03_ex_04.py
#
# Sobel Operator
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

# Sobel Operatot 3 x 3
edges100_75 = cv2.Canny(image, 100, 75)
edges1_255 = cv2.Canny(image, 1, 255)
edges220_225 = cv2.Canny(image, 220, 225)
edges1_128 = cv2.Canny(image, 1, 128)

cv2.namedWindow( "Canny - edges100_75", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Canny - edges100_75", edges100_75 )

cv2.namedWindow( "Canny - edges1_255", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Canny - edges1_255", edges1_255 )

cv2.namedWindow( "Canny - edges220_225", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Canny - edges220_225", edges220_225 )

cv2.namedWindow( "Canny - edges1_128", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Canny - edges1_128", edges1_128 )

cv2.waitKey(0)


