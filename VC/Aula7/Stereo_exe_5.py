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
def drawLines(img, lines, color):
    _, c, _ = img.shape
    print(lines)
    for r in lines:
        x0, y0 = map(int, [0, -r[2]/r[1]])
        x1, y1 = map(int, [c, -(r[2]+r[0]*c)/r[1]])
        cv2.line(img, (x0, y0), (x1, y1), color, 1)

#images_left = sorted(glob.glob('./images/left*.jpg'))
#images_right = sorted(glob.glob('./images/right*.jpg'))
images_left = sorted(glob.glob('./images/StereoL*.bmp'))
images_right = sorted(glob.glob('./images/StereoR*.bmp'))

img_l = cv2.imread(images_left[1])
img_r = cv2.imread(images_right[1])
scaled_camera_matrix, roi = cv2.getOptimalNewCameraMatrix(intrinsics1, distortion1, img_l.shape[0:2], 1, img_l.shape[0:2])
#img_l = cv2.undistort(img_l, intrinsics1, distortion1, None, scaled_camera_matrix)
scaled_camera_matrix, roi = cv2.getOptimalNewCameraMatrix(intrinsics2, distortion2, img_r.shape[0:2], 1, img_r.shape[0:2])
#img_r= cv2.undistort(img_r, intrinsics2, distortion2, None, scaled_camera_matrix)
cv2.imshow('left', img_l)
cv2.imshow('right', img_r)
width, height = img_l.shape[1], img_l.shape[0]

#stereo rectification
R1 = np.zeros((3, 3))
R2 = np.zeros((3, 3))
P1 = np.zeros((3, 4))
P2 = np.zeros((3, 4))
Q = np.zeros((4, 4))

cv2.stereoRectify(intrinsics1, distortion1, intrinsics2, distortion2 ,(width, height), R, T, R1, R2, P1, P2, Q, flags=cv2.CALIB_ZERO_DISPARITY, alpha=-1, newImageSize=(0,0))

#Map computation

def mouse_handler(event, x, y, flags, params):
    if event == cv2.EVENT_LBUTTONDOWN:
        print("left click")
        color = np.random.randint(0, 255, 3).tolist()
        print("coordinates: ",x, y)
        print("InitUndistortRectifyMap");
        map1x, map1y = cv2.initUndistortRectifyMap(intrinsics1, distortion1, R1, P1, (width, height), cv2.CV_32FC1)
        print("map1: " ,map1x, map1y)
        map2x, map2y = cv2.initUndistortRectifyMap(intrinsics2, distortion2, R2, P2, (width, height), cv2.CV_32FC1)
        cv2.remap()
        print("map2: ",map2x, map2y)
        #draw point on the left image
        cv2.circle(img_l, (x, y), 5, color, -1)
        lines = cv2.computeCorrespondEpilines(np.array([[[x, y]]]), 1, F)
        lines = lines.reshape(-1, 3)
        drawLines(img_l, lines, color)
        #draw point on the right image using the map
        cv2.circle(img_r, (int(map2x[y, x]), int(map2y[y, x])), 5, color, -1)
        lines = cv2.computeCorrespondEpilines(np.array([[[int(map2x[y, x]), int(map2y[y, x])]]]), 2, F)
        lines = lines.reshape(-1, 3)
        drawLines(img_r, lines, color)
        cv2.imshow('left', img_l)
        cv2.imshow('right', img_r)
        

cv2.namedWindow('left')
cv2.setMouseCallback('left', mouse_handler)
cv2.waitKey(0)