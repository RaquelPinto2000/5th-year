import numpy as np
import cv2
import glob
import cv2

# Board Size
board_h = 9
board_w = 6

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


def draw(img, corners, imgpts):
    corner = tuple(corners[0].ravel())
    img = cv2.line(img, corner, tuple(imgpts[0].ravel()), (255,0,0), 5)
    img = cv2.line(img, corner, tuple(imgpts[1].ravel()), (0,255,0), 5)
    img = cv2.line(img, corner, tuple(imgpts[2].ravel()), (0,0,255), 5)
    return img

def drawBox(img, imgpts):
    imgpts = np.int32(imgpts).reshape(-1,2)
    # draw ground floor in green
    img = cv2.drawContours(img, [imgpts[:4]],-1,(0,255,0),3)
    # draw pillars in blue color
    for i,j in zip(range(4),range(4,8)):
        img = cv2.line(img, tuple(imgpts[i]), tuple(imgpts[j]),(255),3)
    # draw top layer in red color
    img = cv2.drawContours(img, [imgpts[4:]],-1,(0,0,255),3)
    return img


window_size = (5 ,5)
zero_zone = (-1,-1)
criteria = None

with np.load('camera.npz') as data:
    intrinsics = data['intrinsics']
    distortion = data['distortion']
    rvecs = data['rvecs']
    tvecs = data['tvecs']
print(intrinsics)
print(distortion)
print(rvecs)
print(tvecs)

objp = np.zeros((board_h*board_w,3), np.float32)
objp[:,:2] = np.mgrid[0:board_w,0:board_h].T.reshape(-1,2)
axis = np.float32([[3,0,0], [0,3,0], [0,0,-3]]).reshape(-1,3)
axisBoxes = np.float32([[0,0,0], [0,3,0], [3,3,0], [3,0,0],[0,0,-3],[0,3,-3],[3,3,-3],[3,0,-3]]).reshape(-1,3)
    

#read image
images = glob.glob('./images/left*.jpg')
for fname in images:
    img = cv2.imread(fname)
    gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    cv2.imshow('img',img)
    ret, corners = FindAndDisplayChessboard(img)
    if ret == True:
        corners2 = cv2.cornerSubPix(gray, corners, winSize=window_size, zeroZone=zero_zone, criteria=criteria)
        ret, rvecs, tvecs = cv2.solvePnP(objp, corners2, intrinsics, distortion)
        imgpts, jac = cv2.projectPoints(axisBoxes, rvecs, tvecs, intrinsics, distortion)
        img = drawBox(img, imgpts)
        cv2.imshow('img',img)
        cv2.waitKey(0)
