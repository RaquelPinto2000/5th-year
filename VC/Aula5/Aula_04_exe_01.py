#import
import sys
import numpy as np
import cv2


# Read the image from argv
image = cv2.imread( sys.argv[1] , 0);
#image gray = branco e preto e cizento
ret,thresh_bin = cv2.threshold(image,120,255,cv2.THRESH_BINARY) #converte a imagem para imagem binaria - 0 e 1 (branco e preto)
ret,thresh_bin_inv = cv2.threshold(image,120,255,cv2.THRESH_BINARY_INV) # imagem invertida

#dilation operation using the same structuring element
kernel_circular = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (11,11))

kernel_square = np.ones((11,11),np.uint8)


cv2.imshow( 'image original', image )

cv2.imshow( 'thresh_bin', thresh_bin )

cv2.imshow( 'thresh_bin_inv', thresh_bin_inv )

for i in range(1,4):
    img_dilation_square = cv2.dilate(thresh_bin_inv, kernel_square, iterations=i)
    img_dilation_circular = cv2.dilate(thresh_bin_inv, kernel_circular, iterations=i)
    cv2.imshow( 'img_dilation_square', img_dilation_square )
    cv2.imshow( 'img_dilation_circular', img_dilation_circular ) 
    cv2.waitKey(0)