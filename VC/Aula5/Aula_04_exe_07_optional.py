import sys
import numpy as np
import cv2

seedPoint = (430,30)
# Read the image from argv
image = cv2.imread( sys.argv[1] , 0);
def click(event,x,y,flags,param):
    if event == cv2.EVENT_LBUTTONDBLCLK:
        seedPoint = (x,y)
        #retval, rect = cv2.floodFill(image, mask, seedPoint, newVal[, loDiff[, upDiff[, flags]]])
        height, width = image.shape
        mask = np.zeros((height+2,width+2),np.uint8) #usar a iamgem toda
        cv2.floodFill(image, mask, seedPoint, 0, 5, 5)
        # vai por os valores a 0 e preenche a regiao enquando a margem for 5 valores a cima ou 5 a baixo 
cv2.namedWindow('Flood-Filling')
cv2.setMouseCallback('Flood-Filling', click)

while(True):
    cv2.imshow( 'Flood-Filling', image ) 
    if cv2.waitKey(20) & 0xFF == 27:
        break
cv2.destroyAllWindows()
