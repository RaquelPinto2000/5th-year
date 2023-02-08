import sys
import numpy as np
import cv2

# Read the image from argv
image = cv2.imread( sys.argv[1] , 0);

seedPoint = (430,30)
#retval, rect = cv2.floodFill(image, mask, seedPoint, newVal[, loDiff[, upDiff[, flags]]])
height, width = image.shape

mask = np.zeros((height+2,width+2),np.uint8) #usar a iamgem toda
cv2.floodFill(image, mask, seedPoint, 0, 5, 5)
# vai por os valores a 0 e preenche a regiao enquando a margem for 5 valores a cima ou 5 a baixo 

cv2.imshow( 'Flood-Filling', image ) 

cv2.waitKey(0)

# o objetivo e carregar na imagem 