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

# Average filter 3 x 3
imageAFilter3x3_1 = cv2.blur( image, (3, 3))
imageAFilter3x3_2 = cv2.blur( imageAFilter3x3_1, (3, 3))
imageAFilter3x3_3 = cv2.blur( imageAFilter3x3_2, (3, 3))
cv2.namedWindow( "Average Filter 3 x 3 - 1 Iter", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Average Filter 3 x 3 - 1 Iter", imageAFilter3x3_1 )
cv2.namedWindow( "Average Filter 3 x 3 - 2 Iter", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Average Filter 3 x 3 - 2 Iter", imageAFilter3x3_2 )
cv2.namedWindow( "Average Filter 3 x 3 - 3 Iter", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Average Filter 3 x 3 - 3 Iter", imageAFilter3x3_3 )
##quando mais interacoes se fizer vem menos prontos brancos mas a imagem em geral fica mais desfocada


# Average filter 5 x 5
imageAFilter5x5_1 = cv2.blur( image, (5, 5))
cv2.namedWindow( "Average Filter 5 x 5 - 1 Iter", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Average Filter 5 x 5 - 1 Iter", imageAFilter5x5_1 )

# Average filter 7 x 7
imageAFilter7x7_1 = cv2.blur( image, (7, 7))
cv2.namedWindow( "Average Filter 7 x 7 - 1 Iter", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Average Filter 7 x 7 - 1 Iter", imageAFilter7x7_1 )


cv2.waitKey(0)


