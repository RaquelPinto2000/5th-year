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
imageSobel3x3_X = cv2.Sobel(image, cv2.CV_64F, 1, 0, 3)
imageSobel3x3_Y = cv2.Sobel(image, cv2.CV_64F, 0, 1, 3)
result = (np.square(imageSobel3x3_X) + np.square(imageSobel3x3_Y)) **.5 #**.5 = raiz quadrada da soma do x e y ao ²

#result = cv2.addWeighted(imageSobel3x3_X**2, 0.5, imageSobel3x3_Y**2, 0.5, 0)
#X
cv2.namedWindow( "Sobel 3 x 3 - X", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Sobel 3 x 3 - X", imageSobel3x3_X )
image8bits = np.uint8( np.absolute(imageSobel3x3_X) )
cv2.imshow( "8 bits - Sobel 3 x 3 - X", image8bits )
#Y
cv2.namedWindow( "Sobel 3 x 3 - Y", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Sobel 3 x 3 - Y", imageSobel3x3_Y )
image8bits = np.uint8( np.absolute(imageSobel3x3_Y) )
cv2.imshow( "8 bits - Sobel 3 x 3 - Y", image8bits )
#result
cv2.namedWindow( "Sobel 3 x 3 - XY", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Sobel 3 x 3 - XY", result )
image8bits = np.uint8( np.absolute(result) )
cv2.imshow( "8 bits - Sobel 3 x 3 - XY", image8bits )


# Sobel Operatot 5 x 5 
imageSobel3x3_X = cv2.Sobel(image, cv2.CV_64F, 1, 0, 5)
imageSobel3x3_Y = cv2.Sobel(image, cv2.CV_64F, 0, 1, 5)
result = (np.square(imageSobel3x3_X) + np.square(imageSobel3x3_Y)) **.5 #**.5 = raiz quadrada da soma do x e y ao ²

#result = cv2.addWeighted(imageSobel3x3_X**2, 0.5, imageSobel3x3_Y**2, 0.5, 0)
#X
cv2.namedWindow( "Sobel 5 x 5 - X", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Sobel 5 x 5 - X", imageSobel3x3_X )
image8bits = np.uint8( np.absolute(imageSobel3x3_X) )
cv2.imshow( "8 bits - Sobel 5 x 5 - X", image8bits )
#Y
cv2.namedWindow( "Sobel 5 x 5 - Y", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Sobel 5 x 5 - Y", imageSobel3x3_Y )
image8bits = np.uint8( np.absolute(imageSobel3x3_Y) )
cv2.imshow( "8 bits - Sobel 5 x 5 - Y", image8bits )
#result
cv2.namedWindow( "Sobel 5 x 5 - XY", cv2.WINDOW_AUTOSIZE )
cv2.imshow( "Sobel 5 x 5 - XY", result )
image8bits = np.uint8( np.absolute(result) )
cv2.imshow( "8 bits - Sobel 5 x 5 - XY", image8bits )



cv2.waitKey(0)


