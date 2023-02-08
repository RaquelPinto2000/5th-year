
#import
import numpy as np
import cv2
import sys

def mouse_handler(event, x, y, flags, params):
    if event == cv2.EVENT_LBUTTONDOWN:
        print("left click")
    if event == cv2.EVENT_RBUTTONDOWN:
        print("right click")
        center_coordinates=(x,y)
        radius = 20
        color = (255, 0, 0)
        thickness = 2 # circulo cheio = -1
        #desenhar um circulo
        cv2.circle(image, center_coordinates, radius, color, thickness)
        cv2.imshow( "Display window", image)


# Read the images
image = cv2.imread( sys.argv[1], cv2.IMREAD_UNCHANGED );

# Create a vsiualization window (optional)
cv2.namedWindow( "Display window", cv2.WINDOW_AUTOSIZE )

# Show the image
cv2.imshow( "Display window", image )


#associate the callback to each window
cv2.setMouseCallback("Display window", mouse_handler)

#wait
cv2.waitKey( 0 );
# Destroy the window -- might be omitted
cv2.destroyWindow( "Display window" )

