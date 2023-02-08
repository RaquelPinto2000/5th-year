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


#dilation operation using the same structuring element
kernel_circular = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (11,11))
kernel_square = np.ones((9,9),np.uint8)


img_erode_square = cv2.erode(thresh_bin_inv, kernel_square, iterations=1) # erode = Erosion
img_erode_circular = cv2.erode(thresh_bin_inv, kernel_circular, iterations=2)
cv2.imshow( 'img_erode_square 9_9 iterations=1', img_erode_square )
cv2.imshow( 'img_erode_circular 11_11 iterations=2', img_erode_circular ) 

cv2.waitKey(0)