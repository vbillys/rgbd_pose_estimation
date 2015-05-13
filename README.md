%YAML:1.0
#total # of points in world coordinate system 
total: 100
#outlier ratio of correspondences [0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9]
outlier: [0.9]
#noise level of 2-3 correspondences, unit pixel, [3, 6,  9,  12,  15]
noise_2d: [9]
#noise level of 3-3 correspondences, unit meter, [0.05, 0.075, 0.1, 0.125, 0.15] 
noise_3d: [0.1]
#noise level of N-N correspondences, unit radian, [2, 4, 6, 8, 10]
noise_normal:[6]
#RANSAC iterations: [200,1000,10000,100000]
iteration: [200]
#outlier rejection threshold for 2-3, unit radian, 0.0005
thre_2d: 0.001
#outlier rejection threshold for 3-3, unit meter, 0.1
thre_3d: 0.2
#outlier rejection threshold for N-N, unit radian, 0.001
normal_thre: 0.1

#number of indepdent random test
test_n:300 
#The type of noise models
#noise_model: Uniform
#noise_model: Gaussian
noise_model: Kinect

