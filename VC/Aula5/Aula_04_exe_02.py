#import
import sys
import numpy as np
import cv2


# Read the image from argv
image = cv2.imread( sys.argv[1] , 0);
#image gray = branco e preto e cizento
ret,thresh_bin = cv2.threshold(image,120,255,cv2.THRESH_BINARY) #converte a imagem para imagem binaria - 0 e 1 (branco e preto)
ret,thresh_bin_inv = cv2.threshold(image,120,255,cv2.THRESH_BINARY_INV) # imagem invertida

cv2.imshow( 'image original', image )
cv2.imshow( 'thresh_bin', thresh_bin )
cv2.imshow( 'thresh_bin_inv', thresh_bin_inv )


#dilation operation 
kernel_square = np.ones((3,3),np.uint8)

img_dilation_square = cv2.dilate(thresh_bin_inv, kernel_square, iterations=1)
image_final = cv2.subtract(img_dilation_square, thresh_bin_inv) # img_dilation_square- thresh_bin_inv -> subtrair a imagem a a imagem square

cv2.imshow( 'img_dilation_square', img_dilation_square )
cv2.imshow( 'image_final', image_final )

kernel_square_11 = np.ones((11,11),np.uint8)

img_dilation_square = cv2.dilate(thresh_bin_inv, kernel_square_11, iterations=1)
image_final_11 = cv2.subtract(img_dilation_square, thresh_bin_inv) # img_dilation_square- thresh_bin_inv -> subtrair a imagem a a imagem square

cv2.imshow( 'image_final 11', image_final_11 )
cv2.waitKey(0)
