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
board_h = 10
board_w = 7

# Arrays to store object points and image points from all the images.
objpoints = [] # 3d point in real world space
rcorners = []
lcorners = []
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

def  FindAndDisplayChessboard(img):
    # Find the chess board corners
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    #find chessboard corners for stereo calibration
    ret, corners = cv2.findChessboardCorners(gray, (board_w, board_h), None)
    # If found, display image with corners
    if ret == True:
       # cv2.cornerSubPix(gray, corners, (11, 11), (-1, -1),criteria)
        img = cv2.drawChessboardCorners(img, (board_w, board_h), corners, ret)
        cv2.imshow('img', img)
        cv2.waitKey(500)

    return ret, corners

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((board_w*board_h,3), np.float32)
objp[:,:2] = np.mgrid[0:board_w,0:board_h].T.reshape(-1,2)

# Read two images
images_left = sorted(glob.glob('./images/StereoL*.bmp'))
images_right = sorted(glob.glob('./images/StereoR*.bmp'))
#images_left = sorted(glob.glob('./images/left*.jpg'))
#images_right = sorted(glob.glob('./images/right*.jpg'))
#size of the image
img_size = cv2.imread(images_left[0]).shape[0:2]

for fname_left, fname_right in zip(images_left, images_right):
    img_l = cv2.imread(fname_left)
    img_r = cv2.imread(fname_right)
    gray_l = cv2.cvtColor(img_l, cv2.COLOR_BGR2GRAY)
    gray_r = cv2.cvtColor(img_r, cv2.COLOR_BGR2GRAY)
    print(fname_left, fname_right)
    ret, left_corners= FindAndDisplayChessboard(img_l)
    ret, right_corners= FindAndDisplayChessboard(img_r)
    if ret == True:
        objpoints.append(objp)
        cv2.cornerSubPix(gray_l,left_corners,(11,11),(-1,-1),criteria)
        lcorners.append(left_corners)
        cv2.cornerSubPix(gray_r,right_corners,(11,11),(-1,-1),criteria)
        rcorners.append(right_corners)



# Stereo Calibration  CV_CALIB_SAME_FOCAL_LENGTH
ret, intrinsics1, distortion1, intrinsics2, distortion2, R, T, E, F = cv2.stereoCalibrate(objpoints, lcorners, rcorners, None, None, None, None, img_size, flags=cv2.CALIB_SAME_FOCAL_LENGTH)

# Save the calibration in a file
np.savez("stereoParams.npz",intrinsics1=intrinsics1,distortion1=distortion1,intrinsics2=intrinsics2,distortion2=distortion2,R=R, T=T, E=E, F=F)
print("Calibration saved!")
cv2.waitKey(-1)
cv2.destroyAllWindows()


# funciona