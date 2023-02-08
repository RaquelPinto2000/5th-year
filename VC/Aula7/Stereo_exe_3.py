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

# Board Size
board_h = 9 #10
board_w = 6 #7

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
rcorners = []
lcorners = []

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((board_w*board_h,3), np.float32)
objp[:,:2] = np.mgrid[0:board_w,0:board_h].T.reshape(-1,2)

# Read two images
#images_left = sorted(glob.glob('./images/StereoL*.bmp'))
#images_right = sorted(glob.glob('./images/StereoR*.bmp'))
images_left = sorted(glob.glob('./images/left*.jpg'))
images_right = sorted(glob.glob('./images/right*.jpg'))

#size of the image
img_size_l = cv2.imread(images_left[0]).shape[0:2]
img_size_r = cv2.imread(images_right[0]).shape[0:2]

with np.load('stereoParams.npz') as data:
    intrinsics1= data['intrinsics1']
    distortion1= data['distortion1']
    intrinsics2= data['intrinsics2']
    distortion2= data['distortion2']
    R= data['R']
    T= data['T']
    E= data['E']
    F= data['F']
print(intrinsics1)
print(intrinsics2)
print(distortion1)
print(distortion2)
print(R)
print(T)
print(E)
print(F)


for fname in images_left:
    img = cv2.imread(fname)
    print(fname)
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(intrinsics1, distortion1, img_size_l, 1, img_size_l)
    dst_l = cv2.undistort(img, intrinsics1, distortion1, None, newcameramtx)
    cv2.imshow('image left' ,img)
    cv2.imshow('image left undistorted' ,dst_l)
    cv2.waitKey(-1)
        
for fname in images_right:
    img = cv2.imread(fname)
    print(fname)
    newcameramtx, roi = cv2.getOptimalNewCameraMatrix(intrinsics2, distortion2, img_size_r, 1, img_size_r)
    dst_r = cv2.undistort(img, intrinsics2, distortion2, None, newcameramtx)
    cv2.imshow('image right' ,img)
    cv2.imshow('image right undistorted' ,dst_r)
    cv2.waitKey(-1)

cv2.waitKey(-1)
cv2.destroyAllWindows()