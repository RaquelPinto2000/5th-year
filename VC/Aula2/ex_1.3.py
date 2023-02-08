#import
import numpy as np
import cv2
import sys

# Read the image
image = cv2.imread( sys.argv[1], cv2.IMREAD_UNCHANGED );

#Copy the image
copy = image.copy();

#
rows,cols = image.shape

for i in range(rows):
    for j in range(cols):
        if copy[i,j] <128:
            copy[i,j]=0
    
      

# Create a vsiualization window (optional)
# CV_WINDOW_AUTOSIZE : window size will depend on image size
cv2.namedWindow( "Display window", cv2.WINDOW_AUTOSIZE )
cv2.namedWindow( "Display window copy", cv2.WINDOW_AUTOSIZE )
# Show the image
cv2.imshow( "Display window", image )
cv2.imshow( "Display window copy", copy )
# Wait
cv2.waitKey( 0 );

# Destroy the window -- might be omitted
cv2.destroyWindow( "Display window" )
cv2.destroyWindow( "Display window copy" )