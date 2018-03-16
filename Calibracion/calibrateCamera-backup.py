# -*- coding: utf-8 -*-
"""
Created on Wed Jul 05 17:21:38 2017

@author: Hp
"""
import numpy as np
import cv2
import glob
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

def calibrateCamera(chessboard,ImageDirectory,save=True,showImages=False):
    
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
    
    # prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
    objp = np.zeros((chessboard[0]*chessboard[1],3), np.float32)
    objp[:,:2] = np.mgrid[0:chessboard[0],0:chessboard[1]].T.reshape(-1,2)
    
    # Arrays to store object points and image points from all the images.
    objpoints = [] # 3d point in real world space
    imgpoints = [] # 2d points in image plane.

    images = glob.glob(str(ImageDirectory)+"/*.tiff")    
    
    for name in images:
        print name
        im = cv2.imread(name)
        gray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)

        # Find the chess board corners
        ret, corners = cv2.findChessboardCorners(gray, chessboard)
        print ret
        if(ret == True):
            objpoints.append(objp)
    
            corners2 = cv2.cornerSubPix(gray,corners,(11,11),(-1,-1),criteria)
            imgpoints.append(corners2)
            
            if(showImages):
                # Draw and display the corners
                gray = cv2.drawChessboardCorners(gray, chessboard, corners2,ret)
                cv2.imshow(str(name),gray)
                cv2.waitKey(0)
                
        cv2.destroyAllWindows()
    ret, matrix, dist, rotation, translation = cv2.calibrateCamera(objpoints, imgpoints, gray.shape[::-1],None,None)
    
    np.savez(str(ImageDirectory)+"./cameraCalibration",matrix=matrix,dist=dist, 
             rotation=rotation, translation=translation,imgpoints=imgpoints,
             objpoints=objpoints)

    return matrix, dist, rotation, translation,objpoints, imgpoints, objpoints
    
def calibrateStereoCamera(im1points,im2points, objpoints, cam1Matrix, cam2Matrix, cam1Dist, cam2Dist,imageSize):

     retval, cameraMatrix1, distCoeffs1, cameraMatrix2, distCoeffs2, R, T, E, F = cv2.stereoCalibrate(objpoints, im1points, im2points,cam1Matrix,cam1Dist,cam2Matrix,cam2Dist,imageSize)
     
     r1 = np.array([])
     r2 = np.array([])
     p1 = np.array([])
     p2 = np.array([])
     q = np.array([])
     validPixROI1 = 0
     validPixROI2 = 0
     
     r1,r2,p1,p2,q,validPixROI1,validPixROI2 = cv2.stereoRectify(cam1Matrix,cam1Dist,cam2Matrix,cam2Dist,imageSize, R,T,r1,r2,p1,p2,q,validPixROI1,validPixROI2)
     
     np.savez("./stereoCamCalibration",r1=r1,r2=r2,p1=p1,p2=p2,q=q,validPixROI1=validPixROI1,validPixROI2=validPixROI2)
     return r1,r2,p1,p2,q,validPixROI1,validPixROI2

def undistordRectify(im1,im2,cam1Matrix,cam1Dist,cam2Matrix,cam2Dist,r1,r2,p1,p2,show=False):
    
    mapx1, mapy1 = cv2.initUndistortRectifyMap(cam1Matrix, cam1Dist, r1, p1[:,:3],
                                               im1.shape[:2][::-1],
                                               cv2.CV_32F)
    mapx2, mapy2 = cv2.initUndistortRectifyMap(cam2Matrix, cam2Dist, r2, p2[:,:3],
                                                   im2.shape[:2][::-1],
                                                   cv2.CV_32F)
    
    img_rect1 = cv2.remap(im1, mapx1, mapy1, cv2.INTER_LINEAR)
    img_rect2 = cv2.remap(im2, mapx2, mapy2, cv2.INTER_LINEAR)
 
    # draw the images side by side
    total_size = (max(img_rect1.shape[0], img_rect2.shape[0]),
                  img_rect1.shape[1] + img_rect2.shape[1], 3)
    
    img = np.zeros(total_size, dtype=np.uint8)
    img[:img_rect1.shape[0], :img_rect1.shape[1]] = img_rect1
    img[:img_rect2.shape[0], img_rect1.shape[1]:] = img_rect2
    
    if(show):
        # draw horizontal lines every 25 px accross the side by side image
        for i in range(20, img.shape[0], 25):
            cv2.line(img, (0, i), (img.shape[1], i), (255, 0, 0))
        cv2.imshow('imgRectified', img)
        
    cv2.imwrite("./1.png",img_rect1)
    cv2.imwrite("./2.png",img_rect2)
    
    return img_rect1,img_rect2

def undistort(image,mtx,dist,showImages=False):
    """
    Undistorts the image using the matrix and the distance matrix given
    """
    h,  w = image.shape[:2]
    
    newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))
    
    dst = cv2.undistort(image, mtx, dist, None, newcameramtx)
    
    x,y,w,h = roi
    dst = dst[y:y+h, x:x+w]
    
    if(showImages):
        cv2.imshow("undistort",dst)
        cv2.waitKey(0)
    return dst

def undistortFromFile(image,calibrationDirectory,showImages=False):
    
    calibration = np.load(str(calibrationDirectory)+"./cameraCalibration.npz")
    
    h,  w = image.shape[:2]
    
    newcameramtx, roi=cv2.getOptimalNewCameraMatrix(calibration["matrix"],calibration["dist"],(w,h),1,(w,h))
    
    dst = cv2.undistort(image, calibration["matrix"],calibration["dist"], None, newcameramtx)
    
    x,y,w,h = roi
    dst = dst[y:y+h, x:x+w]
    
    if(showImages):
        cv2.imshow("undistort",dst)
        cv2.waitKey(0)
    return dst
    
def disparityImage(imR,imL,q,params):
    
    window_size = params["windowSize"]
    min_disp =  params["min_disp"]
    num_disp = params["num_disp"]-min_disp
    stereo = cv2.StereoSGBM_create(minDisparity = min_disp,
        numDisparities = num_disp,
        blockSize =  params["blockSize"],
        P1 = 8*3*window_size**2,
        P2 = 32*3*window_size**2,
        disp12MaxDiff =  params["disp12MaxDiff"],
        uniquenessRatio = params["uniquenessRatio"],
        speckleWindowSize = params["speckleWindowSize"],
        speckleRange = params["speckRange"]
    )
    
    disp = stereo.compute(imL, imR).astype(np.float32) / 16.0
    
    cv2.imshow('disparity', (disp-min_disp)/num_disp)
    cv2.imwrite("./disparity.png",((disp-min_disp)/num_disp)*255)
    cv2.waitKey()
    cv2.destroyAllWindows()
    
    
    return disp

if __name__ == "__main__":
    chessboard = (11,7)
    cam1Matrix, cam1Dist, __, __,objpoints, im1points, objpoints = calibrateCamera(chessboard,"./Camera Right")
    cam2Matrix, cam2Dist, __, __,objpoints, im2points, objpoints = calibrateCamera(chessboard,"./Camera Left")
    
    r1,r2,p1,p2,q,validPixROI1,validPixROI2 = calibrateStereoCamera(im1points,im2points, objpoints, cam1Matrix, cam2Matrix, cam1Dist, cam2Dist,(2592,1944))

#    cameraR = np.load("./Camera Right"+"./cameraCalibration.npz")
#    cameraL = np.load("./Camera Left"+"./cameraCalibration.npz")
    
#    parameters = np.load("./stereoCamCalibration.npz")    

    imL = cv2.imread("./left.png")
    imR = cv2.imread("./right.png")
    
    imL1 = undistortFromFile(imL,"./Camera Left")
    
    cv2.imwrite("./test.png",imL1)
    
    undistordRectify(imL,imR,cam2Matrix,cam2Dist,cam1Matrix,cam2Dist,r2,r1,p2,p1)

#    imR = cv2.cvtColor(imR,cv2.COLOR_BGR2GRAY)
#    imL = cv2.cvtColor(imL,cv2.COLOR_BGR2GRAY)
#    
#    disparityParams = { "windowSize": 4, 
#               "min_disp" : 16,
#               "num_disp" : 240,
#               "blockSize" : 16,
#               "disp12MaxDiff": 1,
#               "uniquenessRatio" : 15,
#               "speckleWindowSize" : 100,
#               "speckRange" : 32
#            }
#    disp = disparityImage(imR,imL,parameters["q"],disparityParams)
#    
    
    
    
    
    
    
    
    