#import
import sys
import numpy as np
import cv2


# Read the image from argv
image = cv2.imread( sys.argv[1] , 0);
#image gray = branco e preto e cizento
ret,thresh_bin = cv2.threshold(image,90,255,cv2.THRESH_BINARY) #converte a imagem para imagem binaria - 0 e 1 (branco e preto)

cv2.imshow( 'image original', image )
cv2.imshow( 'thresh_bin', thresh_bin )


#dilation operation using the same structuring element
kernel_circular = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (11,11))


img_erode_circular = cv2.erode(thresh_bin, kernel_circular, iterations=1)
img_dilation_circular = cv2.dilate(img_erode_circular, kernel_circular, iterations=1)
cv2.imshow( 'morphological opening circular 11 diameter', img_dilation_circular ) 


kernel_square = np.ones((9,3),np.uint8)
img_erode_square9_3 = cv2.erode(thresh_bin, kernel_square, iterations=1) # erode = Erosion
img_dilate_square9_3 = cv2.dilate(img_erode_square9_3, kernel_square, iterations=1) 
cv2.imshow( 'morphological opening square 9_3', img_dilate_square9_3 )

kernel_square = np.ones((3,9),np.uint8)
img_erode_square3_9 = cv2.erode(thresh_bin, kernel_square, iterations=1) # erode = Erosion
img_dilate_square3_9 = cv2.dilate(img_erode_square3_9, kernel_square, iterations=1) 
cv2.imshow( 'morphological opening square 3_9', img_dilate_square3_9 ) 
cv2.waitKey(0)
