# -*- coding: utf-8 -*-
"""
Created on Wed Jul 19 15:57:31 2017

@author: 
"""

import calibrateCamera as cc
reload(cc)
import cv2

def main():
    cameraL = cc.calibrateCamera()
    cameraL.calibrateCamera((11,7),"../../../Dataset/Calib/left",1,True,False)
    print "finish calibL"
    cameraR = cc.calibrateCamera()
    cameraR.calibrateCamera((11,7),"../../../Dataset/Calib/right",1,True,False)
    print "finish calibR"
    
    im = cv2.imread("../../../Dataset/Test/right/1.png")
    cameraR.undistort(im,True)
#    cv2.imshow("imagen",im)
    
    cc.saveNumpyMatrix("cameraLmatrix.txt",cameraL.matrix)
    cc.saveNumpyMatrix("cameraLdist.txt",cameraL.dist)
    cc.saveNumpyMatrix("cameraRmatrix.txt",cameraR.matrix)
    cc.saveNumpyMatrix("cameraRdist.txt",cameraR.dist)
    
#    stereoCalib = cc.calibrateStereoCameras(cameraL,cameraR)
#    stereoCalib.calibrateStereoCamera((1100,1100)) 
    
    


if __name__ == "__main__":
    main()