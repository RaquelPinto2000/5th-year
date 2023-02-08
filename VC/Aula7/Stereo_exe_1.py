# ***********************************************************************************
# Name:           chessboard.py
# Revision:
# Date:           28-10-2019
# Author:         Paulo Dias
# Comments:       ChessBoard Tracking
#
# images         left1.jpg->left19.jpg
# Revision:
# ***********************************************************************************
import numpy as np
import cv2
import glob

# Board Size - depende das imagens
board_h = 10
board_w = 7

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space

def  FindAndDisplayChessboard(img):
    # Find the chess board corners
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    ret, corners = cv2.findChessboardCorners(gray, (board_w,board_h),None)
   # print(ret)
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
left_corners = []
right_corners = []

# Read images
images_left = sorted(glob.glob('./images/StereoL*.bmp'))

images_right = sorted(glob.glob('./images/StereoR*.bmp'))

for fname in images_left:
    img = cv2.imread(fname)
    print(fname)
    ret, corners = FindAndDisplayChessboard(img)
    if ret == True:
        objpoints.append(objp)
        left_corners.append(corners)
        #cv2.imshow('imsg',img)

for fname in images_right:
    img = cv2.imread(fname)
    print(fname)
    ret, corners = FindAndDisplayChessboard(img)
    if ret == True:
        objpoints.append(objp)
        right_corners.append(corners)


cv2.waitKey(-1)
cv2.destroyAllWindows()
