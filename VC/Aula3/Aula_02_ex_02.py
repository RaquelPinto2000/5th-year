import numpy as np
import cv2
import sys

# Read the image
image = cv2.imread(sys.argv[1], cv2.IMREAD_UNCHANGED );

if len(image.shape) < 3: # cizento
    rows,cols = image.shape
    color=(255, 255, 255)
else: #cores
    color=(135, 135, 135)
    rows,cols,_ = image.shape

grid_shape =20
dy, dx = rows/grid_shape, cols/grid_shape

      
# draw vertical lines
for x in np.linspace(start=dx, stop=cols-dx, num=grid_shape -1):
    x = int(round(x))
    cv2.line(image, (x, 0), (x, rows), color=color, thickness=1)

# draw horizontal lines
for y in np.linspace(start=dy, stop=rows-dy, num=grid_shape-1):
    y = int(round(y))
    cv2.line(image, (0, y), (cols, y), color=color, thickness=1)

# Create a vsiualization window (optional)
# CV_WINDOW_AUTOSIZE : window size will depend on image size
cv2.namedWindow( "Display window", cv2.WINDOW_AUTOSIZE )
# Show the image
cv2.imshow( "Display window", image )
# Wait
cv2.waitKey( 0 );

# Destroy the window -- might be omitted
cv2.destroyWindow( "Display window" )