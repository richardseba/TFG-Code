# -*- coding: utf-8 -*-
"""
Created on Tue Jul 04 15:44:39 2017

@author: Hp
"""
import cameraViewer as camview
reload(camview)
import joinImages as join
reload(join)
import calibrateCamera as cc
reload(cc)
import cv2

import os
import pickle as p
import numpy as np
from skimage import io
import matplotlib.pyplot as plt

chessboardSize = 1

def main():
    print "inicio";
    select = 2
    
    if(select == 0):
        #test de camera viewer
        camview.cameraViewer(1)
        
    elif(select == 1):
        #test de join function
        
        plt.close("all")
        
        imL = io.imread("./leftcam.png")
        imR = io.imread("./rightcam.png")

        plt.figure(), plt.imshow(imL)
        plt.figure(), plt.imshow(imR)
        
        offset = join.calculateOffset(imL,imR)
        print offset
        test = join.joinImages(imL,imR,32,mode="progressive")
        
        plt.figure(), plt.imshow(test)
        
    elif(select == 2):
        #test de camera calibration module
        imL = cv2.imread("./test1.png")
        imR = cv2.imread("./right.png")
        
        pathL = "C:\Users\rsegovia\Desktop\Proyecto\VR_interfaz\Camera Left"
        pathL = os.path.relpath(pathL)
        print pathL
        
        cameraL = cc.calibrateCamera()
        cameraL.calibrateCamera((11,7),pathL.replace("\\","/"),chessboardSize)
#        
        cameraR = cc.calibrateCamera()
        cameraR.calibrateCamera((11,7),"C:/Users/rsegovia/Desktop/Proyecto/VR_interfaz/Camera Right",chessboardSize)       
#        
        print "saving camera class" #save camera class
        p.dump(cameraL, open("./camera Left/cameraL", "wb"))
        p.dump(cameraR, open("./camera Right/cameraR", "wb"))
        
#        print "loading camera class" #load camera class
#        cameraL = p.load(open("./camera Left/cameraL", "rb"))
#        cameraR = p.load(open("./camera Right/cameraR", "rb"))
        
#        im3 = cameraL.undistort(imL)
#        cv2.imwrite("./test4.png",im3)
        
        cc.saveNumpyMatrix("cameraLmatrix.txt",cameraL.matrix)
        cc.saveNumpyMatrix("cameraLdist.txt",cameraL.dist)
        cc.saveNumpyMatrix("cameraRmatrix.txt",cameraR.matrix)
        cc.saveNumpyMatrix("cameraRdist.txt",cameraR.dist)
        
#        print "computing Stereo ..."
#        cameraStereo = cc.calibrateStereoCameras(cameraR,cameraL)
#        cameraStereo.calibrateStereoCamera((2592,1944))
#        cameraStereo.undistordRectify(imR,imL,False,True)
#        
#        disparityParams = { "windowSize": 4, 
#               "min_disp" : 16,
#               "num_disp" : 240,
#               "blockSize" : 16,
#               "disp12MaxDiff": 1,
#               "uniquenessRatio" : 15,
#               "speckleWindowSize" : 100,
#               "speckRange" : 32
#            }
#        print "calculating disparity image ..."
#        cameraStereo.disparityImage(imR,imL,disparityParams)
        
    
        
    
if __name__ == "__main__":
    main()