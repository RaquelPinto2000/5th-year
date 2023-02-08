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
print(intrinsics1)
print(distortion1)
print(intrinsics2)
print(distortion2)

#F= [[1.26355479e-07,3.49329779e-05,-7.50642410e-03],[-2.94195019e-05, 1.23820441e-05, -1.25338462e-02],[ 5.47657248e-03,  3.86474595e-03 , 1.00000000e+00]]

print('---------------------------')

print(F)

print('---------------------------')
# draw the provided lines on the image
def drawLines(img, lines, color):
    _, c, _ = img.shape
    print(lines)
    for r in lines:
        x0, y0 = map(int, [0, -r[2]/r[1]])
        x1, y1 = map(int, [c, -(r[2]+r[0]*c)/r[1]])
        cv2.line(img, (x0, y0), (x1, y1), color, 1)

images_left = sorted(glob.glob('./images/StereoL*.bmp'))
images_right = sorted(glob.glob('./images/StereoR*.bmp'))

img_l = cv2.imread(images_left[1])
img_r = cv2.imread(images_right[1])
scaled_camera_matrix, roi = cv2.getOptimalNewCameraMatrix(intrinsics1, distortion1, img_l.shape[0:2], 1, img_l.shape[0:2])
#img_l = cv2.undistort(img_l, intrinsics1, distortion1, None, scaled_camera_matrix)
scaled_camera_matrix, roi = cv2.getOptimalNewCameraMatrix(intrinsics2, distortion2, img_r.shape[0:2], 1, img_r.shape[0:2])
#img_r= cv2.undistort(img_r, intrinsics2, distortion2, None, scaled_camera_matrix)
cv2.imshow('undistort img left', img_l)
cv2.imshow('undistort img right', img_r)

#select on left image, draw on right image
def mouse_handler(event, x, y, flags, params):
    if event == cv2.EVENT_LBUTTONDOWN:
        print("left click")
        color = np.random.randint(0, 255, 3).tolist()
        print(x, ' ', y)
        cv2.circle(img_l, (x, y), 5, color, -1,1)
        cv2.imshow('undistort img left', img_l)
        #find the epipolar line
        lines = cv2.computeCorrespondEpilines(np.array([[[x, y]]]), 1, F)
        lines = lines.reshape(-1, 3)
        drawLines(img_r, lines, color)
        cv2.imshow('undistort img right', img_r)

        
cv2.setMouseCallback('undistort img left', mouse_handler)
cv2.waitKey(-1)

'''
for fname_left, fname_right in zip(images_left, images_right):
    img_l = cv2.imread(fname_left,0)
    img_r = cv2.imread(fname_right,0)

    print(fname_left, fname_right)
    cv2.imshow('img left', img_l)
    scaled_camera_matrix, roi = cv2.getOptimalNewCameraMatrix(intrinsics1, distortion1, img_l.shape[0:2], 1, img_l.shape[0:2])
    img_l_undistort = cv2.undistort(img_l, intrinsics1, distortion1, None, scaled_camera_matrix)
    cv2.imshow('undistort img left', img_l_undistort)
    cv2.waitKey(-1)

    cv2.imshow('img right', img_r)
    scaled_camera_matrix, roi = cv2.getOptimalNewCameraMatrix(intrinsics2, distortion2, img_r.shape[0:2], 1, img_r.shape[0:2])
    img_r_undistort = cv2.undistort(img_r, intrinsics2, distortion2, None, scaled_camera_matrix)
    cv2.imshow('undistort img right', img_r_undistort)
    
    color = np.random.randint(0, 255, 3).tolist()
    cv2.imshow('undistort img right', img_r_undistort)

    cv2.waitKey(-1)
'''

