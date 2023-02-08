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


#dilation operation using the same structuring element
kernel_circular = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (11,11))
kernel_square = np.ones((11,11),np.uint8)

for i in range(1,4):
    img_erode_square = cv2.erode(thresh_bin_inv, kernel_square, iterations=i) # erode = Erosion
    img_erode_circular = cv2.erode(thresh_bin_inv, kernel_circular, iterations=i)
    cv2.imshow( 'img_erode_square 11_11', img_erode_square )
    cv2.imshow( 'img_erode_circular 11_11', img_erode_circular ) 
    cv2.waitKey(0)

#dilation operation using the same structuring element
kernel_square11_1 = np.ones((11,1),np.uint8)

img_erode_square = cv2.erode(thresh_bin_inv, kernel_square11_1, iterations=1) # erode = Erosion
cv2.imshow( 'img_erode_square 11_1', img_erode_square )

#dilation operation using the same structuring element
kernel_square = np.array([[1,1,1],[1,0,1],[1,1,1]],np.uint8) # (“hotspot”)

img_erode_square = cv2.erode(thresh_bin_inv, kernel_square, iterations=1) # erode = Erosion
cv2.imshow( 'img_erode_square 3_3', img_erode_square )
cv2.waitKey(0)