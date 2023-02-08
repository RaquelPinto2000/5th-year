#import
import numpy as np
import cv2
import sys

#exercicio 1.6 opcional

# Read the images
image = cv2.imread( sys.argv[1], cv2.IMREAD_UNCHANGED ); #deti.jpg

image_gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY) 

image_grayHLS = cv2.cvtColor(image, cv2.COLOR_RGB2HLS) #tonalidade, a saturação e o brilho -> espetro de cor com mais tonalidades brancas
image_grayXYZ = cv2.cvtColor(image, cv2.COLOR_RGB2XYZ) #comprimentos de onda no espectro visível eletromagnético e cores percebidas na visão de cores humana
image_grayHSV = cv2.cvtColor(image, cv2.COLOR_RGB2HSV) #tonalidade, Saturação e brilho -> espetro de cor

# Create a vsiualization window (optional)
# CV_WINDOW_AUTOSIZE : window size will depend on image size
cv2.namedWindow( "Display window RGB", cv2.WINDOW_AUTOSIZE )
cv2.namedWindow( "Display window gray", cv2.WINDOW_AUTOSIZE )
cv2.namedWindow( "Display window HLS", cv2.WINDOW_AUTOSIZE )
cv2.namedWindow( "Display window XYZ", cv2.WINDOW_AUTOSIZE )
cv2.namedWindow( "Display window HSV", cv2.WINDOW_AUTOSIZE )

# Show the image
cv2.imshow( "Display window RGB", image )
cv2.imshow( "Display window gray", image_gray )
cv2.imshow( "Display window HLS", image_grayHLS)
cv2.imshow( "Display window XYZ", image_grayXYZ)
cv2.imshow( "Display window HSV", image_grayHSV )
# Wait
cv2.waitKey( 0 );

# Destroy the window -- might be omitted
cv2.destroyWindow( "Display window RGB" )
cv2.destroyWindow( "Display window gray" )
cv2.destroyWindow( "Display window HLS" )
cv2.destroyWindow( "Display window XYZ" )
cv2.destroyWindow( "Display window HSV" )