import numpy as np
import cv2
import glob

#read the distortion parameters of the camera
with np.load('stereoParams.npz') as data:
    intrinsics1 = data['intrinsics1']
    distortion1 = data['distortion1']
    intrinsics2 = data['intrinsics2']
    distortion2 = data['distortion2']
    F = data['F']
    R = data['R']
    T = data['T']
print(intrinsics1)
print(distortion1)
print(intrinsics2)
print(distortion2)

# draw the provided lines on the image
def drawlines(img2, lines, color):
    r, c, _ = img2.shape
    for r in lines:
        x0, y0 = map(int, [0, -r[2]/r[1]])
        x1, y1 = map(int, [c, -(r[2]+r[0]*c)/r[1]])
        img2 = cv2.line(img2, (x0,y0), (x1,y1), color,1)

images_left = sorted(glob.glob('#Lab5and6Images//left*.jpg'))
images_right = sorted(glob.glob('#Lab5and6Images//right*.jpg'))

img_l = cv2.imread(images_left[0])
img_r = cv2.imread(images_right[0])
scaled_camera_matrix, roi = cv2.getOptimalNewCameraMatrix(intrinsics1, distortion1, img_l.shape[0:2], 1, img_l.shape[0:2])
#img_l = cv2.undistort(img_l, intrinsics1, distortion1, None, scaled_camera_matrix)
scaled_camera_matrix, roi = cv2.getOptimalNewCameraMatrix(intrinsics2, distortion2, img_r.shape[0:2], 1, img_r.shape[0:2])
#img_r= cv2.undistort(img_r, intrinsics2, distortion2, None, scaled_camera_matrix)
cv2.imshow('left', img_l)
cv2.imshow('right', img_r)
width, height = img_l.shape[1], img_l.shape[0]

sift = cv2.SIFT_create()
# find the keypoints and descriptors with SIFT
kp1, des1 = sift.detectAndCompute(img_l,None)
kp2, des2 = sift.detectAndCompute(img_r,None)
# FLANN parameters
FLANN_INDEX_KDTREE = 1
index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
search_params = dict(checks=50)
flann = cv2.FlannBasedMatcher(index_params,search_params)
matches = flann.knnMatch(des1,des2,k=2)
pts1 = []
pts2 = []
# ratio test as per Lowe's paper
for i,(m,n) in enumerate(matches):
    if m.distance < 0.8*n.distance:
        pts2.append(kp2[m.trainIdx].pt)
        pts1.append(kp1[m.queryIdx].pt)

pts1 = np.int32(pts1)
pts2 = np.int32(pts2)
F, mask = cv2.findFundamentalMat(pts1,pts2,cv2.FM_LMEDS)

#stereo rectification
R1 = np.zeros((3, 3))
R2 = np.zeros((3, 3))
P1 = np.zeros((3, 4))
P2 = np.zeros((3, 4))
Q = np.zeros((4, 4))

cv2.stereoRectify(intrinsics1, distortion1, intrinsics2, distortion2 ,(width, height), R, T, R1, R2, P1, P2, Q, flags=cv2.CALIB_ZERO_DISPARITY, alpha=-1, newImageSize=(0,0))
map1x, map1y = cv2.initUndistortRectifyMap(intrinsics1, distortion1, R1, P1, (width, height), cv2.CV_32FC1)
map2x, map2y = cv2.initUndistortRectifyMap(intrinsics2, distortion2, R2, P2, (width, height), cv2.CV_32FC1)
cv2.remap(img_l, map1x,map1y,cv2.INTER_LINEAR)
cv2.remap(img_r, map2x,map2y,cv2.INTER_LINEAR)
#Map computation

def mouse_handler_left(event, x, y, flags, params):
    if event == cv2.EVENT_LBUTTONDOWN:
        print("left click")
        color = np.random.randint(0, 255, 3).tolist()
        print("coordinates: ",x, y)
        #draw point on the left image
        cv2.circle(img_l, (x, y), 5, color, -1)
        lines = cv2.computeCorrespondEpilines(np.array([[[x, y]]]), 1, F)
        lines = lines.reshape(-1, 3)
        #draw point on the right image using the map
        drawlines(img_r, lines, color)
        cv2.imshow('left', img_l)
        cv2.imshow('right', img_r)

def mouse_handler_right(event, x, y, flags, params):
    if event == cv2.EVENT_LBUTTONDOWN:
        print("right click")
        color = np.random.randint(0, 255, 3).tolist()
        print("coordinates: ",x, y)
        #draw point on the left image
        cv2.circle(img_r, (x, y), 5, color, -1)
        lines = cv2.computeCorrespondEpilines(np.array([[[x, y]]]), 1, F)
        lines = lines.reshape(-1, 3)
        #draw point on the right image using the map
        drawlines(img_l, lines, color)
        cv2.imshow('left', img_l)
        cv2.imshow('right', img_r)
        

cv2.namedWindow('left')
cv2.setMouseCallback('left', mouse_handler_left)
cv2.namedWindow('right')
cv2.setMouseCallback('right', mouse_handler_right)
cv2.waitKey(0)