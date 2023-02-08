# ***********************************************************************************
# Name:           chessboard.py
# Revision:
# Date:           28-10-2019
# Author:         Paulo Dias
# Comments:       ChessBoard Tracking
#
# images         left1.jpg->left19.jpg
# Revision:
# Libraries:    Python 3.7.3 + openCV 4.1.0
# ***********************************************************************************
import numpy as np
import cv2
import glob

# Board Size
board_h = 9
board_w = 6

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.


def  FindAndDisplayChessboard(img):
    # Find the chess board corners
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    ret, corners = cv2.findChessboardCorners(gray, (board_w,board_h),None)
    # If found, display image with corners
    if ret == True:
        img = cv2.drawChessboardCorners(img, (board_w, board_h), corners, ret)
        cv2.imshow('img',img)
        cv2.waitKey(500)

    return ret, corners

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((board_w*board_h,3), np.float32)
objp[:,:2] = np.mgrid[0:board_w,0:board_h].T.reshape(-1,2)

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.

# Read images
images = glob.glob('./images/left*.jpg')

for fname in images:
    img = cv2.imread(fname)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    ret, corners = FindAndDisplayChessboard(img)
    if ret == True:
        objpoints.append(objp)
        imgpoints.append(corners)


image_size = (img.shape[0], img.shape[1])
ret, intrinsics, distortion,tvecs,rvecs = cv2.calibrateCamera(objpoints,imgpoints, gray.shape[::-1], None, None)

print("Intrinsics: ")
print (intrinsics) # e a matriz que devolve - documentacao
print("Distortion : ")
print(distortion)
for i in range(len(tvecs)):
    print ("Translations(%d) : " % i )
    print(tvecs[0])
    print ("Rotation(%d) : " % i )
    print(rvecs[0])

#guarda num ficheiro as configuracoes
np.savez('camera.npz', intrinsics=intrinsics, distortion=distortion)

cv2.waitKey(-1)
cv2.destroyAllWindows()