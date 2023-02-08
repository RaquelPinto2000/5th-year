#import
import numpy as np
import cv2
import sys

# Read the images
image1 = cv2.imread( sys.argv[1], cv2.IMREAD_UNCHANGED ); #deti.bmp
image2 = cv2.imread( sys.argv[2], cv2.IMREAD_UNCHANGED ); #deti.jpg

image3 = cv2.subtract(image1,image2); #subtraimos os pixeis de uma pelos pixeis da outra para temos as diferencas das imagens

#existe diferencas que sao introduzidas pela compressao ... bmp nao tem e jpg tem

# Create a vsiualization window (optional)
cv2.namedWindow( "Display window", cv2.WINDOW_AUTOSIZE )

# Show the image
cv2.imshow( "Display window", image3 )
#wait
cv2.waitKey( 0 );
# Destroy the window -- might be omitted
cv2.destroyWindow( "Display window" )