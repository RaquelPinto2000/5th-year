#import
import numpy as np
import cv2
import sys

# Read the images
image1 = cv2.imread( sys.argv[1], cv2.IMREAD_UNCHANGED );
cv2.imwrite("imagem2.png", image1, [cv2.IMWRITE_PNG_COMPRESSION, 500])
image2 = cv2.imread( "imagem2.png", cv2.IMREAD_UNCHANGED );
image3 = cv2.subtract(image1,image2);

#existe diferencas que sao introduzidas pela compressao ... bmp nao tem e jpg tem

# Create a vsiualization window (optional)
cv2.namedWindow( "Display window", cv2.WINDOW_AUTOSIZE )
cv2.namedWindow( "Display window1", cv2.WINDOW_AUTOSIZE )

# Show the image
cv2.imshow( "Display window", image3 )
cv2.imshow( "Display window1", image2 )
#wait
cv2.waitKey( 0 );
# Destroy the window -- might be omitted
cv2.destroyWindow( "Display window" )
cv2.destroyWindow( "Display window1" )