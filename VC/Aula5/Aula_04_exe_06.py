#import
import sys
import numpy as np
import cv2


# Read the image from argv
image = cv2.imread( sys.argv[1] , 0);
#image gray = branco e preto e cizento
ret,thresh_bin = cv2.threshold(image,90,255,cv2.THRESH_BINARY) #converte a imagem para imagem binaria - 0 e 1 (branco e preto)
ret,thresh_bin_inv = cv2.threshold(image,90,255,cv2.THRESH_BINARY_INV) # imagem invertida
cv2.imshow( 'image original', image )
cv2.imshow( 'thresh_bin', thresh_bin )
cv2.imshow( 'thresh_bin_inv', thresh_bin_inv )


kernel_circular = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (30,30))
img_dilation_circular = cv2.dilate(thresh_bin_inv, kernel_circular, iterations=1)
img_erode_circular = cv2.erode(img_dilation_circular, kernel_circular, iterations=1)
cv2.imshow( 'morphological opening circular 30_30', img_erode_circular ) 


kernel_circular = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (22,22))
img_dilation_circular = cv2.dilate(thresh_bin_inv, kernel_circular, iterations=1)
img_erode_circular = cv2.erode(img_dilation_circular, kernel_circular, iterations=1)
cv2.imshow( 'morphological opening circular 22_22', img_erode_circular ) 


kernel_circular = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (16,16))
img_dilation_circular = cv2.dilate(thresh_bin_inv, kernel_circular, iterations=1)
img_erode_circular = cv2.erode(img_dilation_circular, kernel_circular, iterations=1)
cv2.imshow( 'morphological opening circular 16_16', img_erode_circular ) 


kernel_circular = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (11,11))
img_dilation_circular = cv2.dilate(thresh_bin_inv, kernel_circular, iterations=1)
img_erode_circular = cv2.erode(img_dilation_circular, kernel_circular, iterations=1)
cv2.imshow( 'morphological opening circular 11_11', img_erode_circular ) 

cv2.waitKey(0)