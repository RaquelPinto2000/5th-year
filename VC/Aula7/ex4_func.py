import cv2
from matplotlib import pyplot as plt
import numpy as np
import glob

images_left = sorted(glob.glob('#Lab5and6Images//left*.jpg'))
images_right = sorted(glob.glob('#Lab5and6Images//right*.jpg'))

img_l = cv2.imread(images_left[0])
img_r = cv2.imread(images_right[0])

img1 = cv2.imread(images_left[0])
img2 = cv2.imread(images_right[0])
cv2.imshow('img1',img1)
cv2.imshow('img2',img2)

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
# We select only inlier points
pts1 = pts1[mask.ravel()==1]
pts2 = pts2[mask.ravel()==1]

def drawlines(img_l,img_r,lines,pts1,pts2):
    ''' img1 - image on which we draw the epilines for the points in img2
        lines - corresponding epilines '''
    r,c,_ = img_l.shape
    for r,pt1,pt2 in zip(lines,pts1,pts2):
        color = tuple(np.random.randint(0,255,3).tolist())
        x0,y0 = map(int, [0, -r[2]/r[1] ])
        x1,y1 = map(int, [c, -(r[2]+r[0]*c)/r[1] ])
        cv2.line(img_l, (x0,y0), (x1,y1), color,1)
        cv2.circle(img_l,tuple(pt1),5,color,-1)
        cv2.circle(img_r,tuple(pt2),5,color,-1)


# Find epilines corresponding to points in right image (second image) and
# drawing its lines on left image
lines1 = cv2.computeCorrespondEpilines(pts2.reshape(-1,1,2), 2,F)
lines1 = lines1.reshape(-1,3)
drawlines(img_l,img_r,lines1,pts1,pts2)
# Find epilines corresponding to points in left image (first image) and
# drawing its lines on right image
lines2 = cv2.computeCorrespondEpilines(pts1.reshape(-1,1,2), 1,F)
lines2 = lines2.reshape(-1,3)
drawlines(img_r,img_l,lines2,pts2,pts1)
#plot both images img_l and img_r
#plt.subplot(121),plt.imshow(img_l)
#plt.subplot(122),plt.imshow(img_r)
#plt.show()
#wait for key and close


#draw lines iteratively select point in left image and draw epipolar line in right image

def drawlines(img1,lines,color):
    ''' img1 - image on which we draw the epilines for the points in img2
        lines - corresponding epilines '''
    _,c,_ = img1.shape
    for r in lines:
        x0, y0 = map(int, [0, -r[2]/r[1]])
        x1, y1 = map(int, [c, -(r[2]+r[0]*c)/r[1]])
        cv2.line(img1, (x0,y0), (x1,y1), color,1)

def draw_epipolar_line(event,x,y,flags,param):
    if event == cv2.EVENT_LBUTTONDOWN:
        print("left click")
        color = np.random.randint(0, 255, 3).tolist()
        print("coordinates: ",x, y)
        #draw point on the left image
        cv2.circle(img1, (x, y), 5, color, -1)
        lines = cv2.computeCorrespondEpilines(np.array([[[x, y]]]), 1, F)
        lines = lines.reshape(-1, 3)
        #draw point on the right image using the map
        drawlines(img2, lines, color)
        cv2.imshow('img1', img1)
        cv2.imshow('img2', img2)

cv2.namedWindow('img1')
cv2.setMouseCallback('img1',draw_epipolar_line)
cv2.imshow('img1',img1)
cv2.imshow('img2',img2)
cv2.waitKey(0)
cv2.destroyAllWindows()


cv2.waitKey(0)