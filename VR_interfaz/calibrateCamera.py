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

class calibrateCamera:
    def __init__(self,matrix=0,dist=0,rotation=0,traslation=0,objpoints=0,imgpoints=0):
        self.matrix = matrix
        self.dist = dist
        self.rotation = rotation 
        self.traslation = traslation
        self.objpoints = objpoints
        self.imgpoints = imgpoints
    
    def calibrateCamera(self,chessboard,ImageDirectory,chessBoardSize,save=True,showImages=False):
    
        criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
        
        # prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
        objp = np.zeros((chessboard[0]*chessboard[1],3), np.float32)
        objp[:,:2] = np.mgrid[0:chessboard[0],0:chessboard[1]].T.reshape(-1,2)
        
        # Arrays to store object points and image points from all the images.
        self.objpoints = [] # 3d point in real world space
        self.imgpoints = [] # 2d points in image plane.
    
        images = glob.glob(str(ImageDirectory)+"/*.tiff")    
        
        for name in images:
            print name
            im = cv2.imread(name)
            gray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
    
            # Find the chess board corners
            ret, corners = cv2.findChessboardCorners(gray, chessboard)
            print ret
            if(ret == True):
                self.objpoints.append(objp*chessBoardSize)
        
                corners2 = cv2.cornerSubPix(gray,corners,(11,11),(-1,-1),criteria)
                self.imgpoints.append(corners2)
                
                if(showImages):
                    # Draw and display the corners
                    gray = cv2.drawChessboardCorners(gray, chessboard, corners2,ret)
                    cv2.imshow(str(name),gray)
                    cv2.waitKey(0)
                    
            cv2.destroyAllWindows()
        ret, self.matrix, self.dist, self.rotation, self.translation = cv2.calibrateCamera(self.objpoints, self.imgpoints, gray.shape[::-1],None,None)
        
        np.savez(str(ImageDirectory)+"./cameraCalibration",matrix=self.matrix,dist=self.dist, 
                 rotation=self.rotation, translation=self.translation,imgpoints=self.imgpoints,
                 objpoints=self.objpoints)
        
    def undistort(self,image,showImages=False):
        """
        Undistorts the image using the matrix and the distance matrix given
        """
        h,  w = image.shape[:2]
        
        newcameramtx, roi=cv2.getOptimalNewCameraMatrix(self.matrix,self.dist,(w,h),1,(w,h))
        
        result = cv2.undistort(image, self.matrix, self.dist, None, newcameramtx)
        
        x,y,w,h = roi
        result = result[y:y+h, x:x+w]
        
        
        if(showImages):
            cv2.imshow("undistort",result)
            cv2.waitKey(0)
        return result    
    

class calibrateStereoCameras:
    def __init__(self,camera1,camera2):
        self.cam1 = camera1
        self.cam2 = camera2
        
    def calibrateStereoCamera(self,imageSize):
        
        #mirar si cambia tanto los dist coeff como las matrices
         __, __, __, __, __, self.R, self.T, self.E, self.F = cv2.stereoCalibrate(self.cam1.objpoints,self.cam1.imgpoints,self.cam2.imgpoints,
                                                                                   self.cam1.matrix,self.cam1.dist,self.cam2.matrix,self.cam2.dist,imageSize)
         
         self.cam1.r = np.array([])
         self.cam2.r = np.array([])
         self.cam1.p = np.array([])
         self.cam2.p = np.array([])
         self.q = np.array([])
         self.cam1.ROI = 0
         self.cam2.ROI = 0
         
         self.cam1.r,self.cam2.r,self.cam1.p,self.cam2.p,self.q,self.cam1.ROI,self.cam2.ROI = cv2.stereoRectify(self.cam1.matrix,self.cam1.dist,self.cam2.matrix,
                                                                                                                  self.cam2.dist,imageSize,self.R,self.T,self.cam1.r,
                                                                                                                  self.cam2.r,self.cam1.p,self.cam2.p,self.q,
                                                                                                                  self.cam1.ROI,self.cam2.ROI)
         
         np.savez("./stereoCamCalibration",r1=self.cam1.r,r2=self.cam2.r,p1=self.cam1.p,p2=self.cam2.p,q=self.q,validPixROI1=self.cam1.ROI,validPixROI2=self.cam2.ROI)
    
    
    def undistordRectify(self,im1,im2,show=False,save=False):
        
        mapx1, mapy1 = cv2.initUndistortRectifyMap(self.cam1.matrix, self.cam1.dist, self.cam1.r, self.cam1.p[:,:3],
                                                   im1.shape[:2][::-1],
                                                   cv2.CV_32F)
        mapx2, mapy2 = cv2.initUndistortRectifyMap(self.cam2.matrix, self.cam2.dist, self.cam2.r, self.cam2.p[:,:3],
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
        
        if(save):
            cv2.imwrite("./1.png",img_rect1)
            cv2.imwrite("./2.png",img_rect2)
        
        return img_rect1,img_rect2
    
    def disparityImage(self,imR,imL,params,save=False,show=False): #falta cambiar
    
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
        
        if(show):
            cv2.imshow('disparity', (disp-min_disp)/num_disp)
            
        if(save):
            cv2.imwrite("./disparity.png",((disp-min_disp)/num_disp)*255)
        
        return disp
    
#Utils

def saveNumpyMatrix(namefile,matrix):
    savefile = open(namefile,"w")
    filecontent = ""
    for row in matrix:
        for element in row:
            filecontent += str(element) + ","
        filecontent = filecontent[:-1]
        filecontent += "\n"
    savefile.write(filecontent)
    savefile.close()
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    