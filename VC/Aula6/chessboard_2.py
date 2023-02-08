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

def draw(img, corners, imgpts):
    imgpts = np.int32(imgpts).reshape(-1,2)
    # draw ground floor in green
    img = cv2.drawContours(img, [imgpts[:4]],-1,(0,255,0),3)
    # draw pillars in blue color
    for i,j in zip(range(4),range(4,8)):
        img = cv2.line(img, tuple(imgpts[i]), tuple(imgpts[j]),(255),3)
    # draw top layer in red color
    img = cv2.drawContours(img, [imgpts[4:]],-1,(0,0,255),3)
    return img

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
window_size = (5 ,5)
zero_zone = (-1,-1)
criteria = None

for fname in images:
    img_frame = cv2.imread(fname)
    gray = cv2.cvtColor(img_frame,cv2.COLOR_BGR2GRAY)
    ret, corners = FindAndDisplayChessboard(img_frame)
    corners = cv2.cornerSubPix(gray, corners, winSize=window_size, zeroZone=zero_zone, criteria=criteria)
    if ret == True:
        objpoints.append(objp)
        imgpoints.append(corners)

ret, intrinsics, distortion, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

img = cv2.imread(images[0])
gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
ret, corners = FindAndDisplayChessboard(img) # faz o que
corners = cv2.cornerSubPix(gray, corners, winSize=window_size, zeroZone=zero_zone, criteria=criteria)
object_points = np.float32(([0,0,0], [0,3,0], [3,3,0], [3,0,0],[0,0,-3],[0,3,-3],[3,3,-3],[3,0,-3])).reshape(-1,3)
imgpts, jac = cv2.projectPoints(object_points, rvecs[0], tvecs[0], intrinsics, distortion)
img = draw(img, corners=corners, imgpts=imgpts)

print("Intrinsics: ")
print (intrinsics)
print("Distortion : ")
print(distortion)
for i in range(len(tvecs)):
    print ("Translations(%d) : " % i )
    print(tvecs[0])
    print ("Rotation(%d) : " % i )
    print(rvecs[0])

cv2.imshow('img',img)
np.savez('camera.npz', intrinsics=intrinsics, distortion=distortion, rvecs=rvecs, tvecs=tvecs)
cv2.waitKey(-1)
cv2.destroyAllWindows()