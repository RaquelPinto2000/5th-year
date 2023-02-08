# ***********************************************************************************
# Name:           viewcloud.py
# Revision:
# Date:           30-10-2019
# Author:         Paulo Dias
# Comments:       Viewcloud
#
# images         left1.jpg->left19.jpg
# Revision:
# Libraries:    Python 3.7.5 + openCV 4.1.0
# ***********************************************************************************
import numpy as np
import open3d as o3d
import copy
import cv2

def draw_registration_result(source, target, transformation):
    source_temp = copy.deepcopy(source)
    target_temp = copy.deepcopy(target)
    #mudar as cores para se ver melhor
    source_temp.paint_uniform_color([1, 0.706, 0])
    target_temp.paint_uniform_color([0, 0.651, 0.929])
    source_temp.transform(transformation)
    o3d.visualization.draw_geometries([source_temp, target_temp])

#EX4
office1= o3d.io.read_point_cloud('./depth_Images/filt_office1.pcd')
office2= o3d.io.read_point_cloud('./depth_Images/filt_office2.pcd')
#o3d.io.write_point_cloud('./depth_Images/office1.pcd', office1)
#o3d.io.write_point_cloud('./depth_Images/office2.pcd', office2)

#mudar as cores para se ver melhor
office1.paint_uniform_color([1, 0.706, 0])
office2.paint_uniform_color([0, 0.651, 0.929])
    
o3d.visualization.draw_geometries([office1,office2])
#o3d.registration_icp.transformation()

#ex5
#icp aligment
source= o3d.io.read_point_cloud('./depth_Images/filt_office1.pcd')
target= o3d.io.read_point_cloud('./depth_Images/filt_office2.pcd')

print("Initial alignment")
#evaluation = o3d.registration.evaluate_registration(source, target, threshold, trans_init)
#print(evaluation)

print("Apply point-to-point ICP")
reg_p2p = o3d.pipelines.registration.registration_icp(source, target, 0.2, np.eye(4),o3d.pipelines.registration.TransformationEstimationPointToPoint())
print(reg_p2p)
print("Transformation is:")
print(reg_p2p.transformation)
draw_registration_result(source, target, reg_p2p.transformation)


