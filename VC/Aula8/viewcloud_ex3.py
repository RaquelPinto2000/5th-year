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

# Create array of random points between [-1,1]
pcl = o3d.geometry.PointCloud()
pcl.points = o3d.utility.Vector3dVector(np.random.rand(2500,3) * 2 - 1)

# Create axes mesh
Axes = o3d.geometry.TriangleMesh.create_coordinate_frame(1)

# shome meshes in view
#o3d.visualization.draw_geometries([pcl , Axes])

# read npz file with 3d points and colors
with np.load('reconstruction.npz') as data:
    points = data['points']
    colors = data['colors']
    normals = data['normals']

p = points.reshape(-1, 3)
fp = []
for i in range(p.shape[0]):
    if np.all(~np.isinf(p[i:])):
        fp.append(p[i])
pcl = o3d.geometry.PointCloud()
pcl.points = o3d.utility.Vector3dVector(fp)

#to improve visualization of the point cloud we can apply a voxel grid filter
pcl = pcl.voxel_down_sample(voxel_size=0.01)

#improve the visualization of the point cloud by applying a radius_outlier_filter
pcl, ind = pcl.remove_radius_outlier(nb_points=16, radius=0.05)

#improve the visualization of the point cloud by applying a statistical_outlier_filter
pcl, ind = pcl.remove_statistical_outlier(nb_neighbors=20, std_ratio=2.0)

#algoritmos
#estimate normals
'''
pcl.estimate_normals(search_param=o3d.geometry.KDTreeSearchParamHybrid(radius=0.1, max_nn=30))

#triangulate the point cloud
print("triangulate the point cloud")
distances = pcl.compute_nearest_neighbor_distance()
avg_dist = np.mean(distances)
radius = 1.5 * avg_dist   
mesh = o3d.geometry.TriangleMesh.create_from_point_cloud_ball_pivoting(pcl, o3d.utility.DoubleVector([radius, radius * 2]))
'''

#texture the mesh
'''
mesh.compute_vertex_normals()
mesh.paint_uniform_color([0.5, 0.5, 0.5])
mesh = mesh.sample_points_uniformly(number_of_points=50000)
print("Paint the mesh")
with o3d.utility.VerbosityContextManager(o3d.utility.VerbosityLevel.Debug) as cm:
    mesh, densities = o3d.geometry.TriangleMesh.create_from_point_cloud_poisson(pcl, depth=9)
print("Computing normal and rendering it.")
mesh.compute_vertex_normals()
mesh.paint_uniform_color([1, 0.706, 0])
print("Visualize the mesh.")

#optional
mesh.remove_degenerate_triangles()
mesh.remove_duplicated_triangles()
mesh.remove_duplicated_vertices()
mesh.remove_non_manifold_edges()
'''
o3d.visualization.draw_geometries([pcl, Axes])
