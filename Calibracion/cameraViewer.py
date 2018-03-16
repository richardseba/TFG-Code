# -*- coding: utf-8 -*-
"""

@author: Hp
"""

import numpy as np
import joinImages
reload(joinImages)
import cv2
import matplotlib.pyplot as plt

cam1height = 100
cam1width = 100

cam2height = 100
cam2width = 100


def cameraViewer(mode=1):

    if(mode==1 or mode == 0):
        camera1 = cv2.VideoCapture(0)
#        camera1.set(3,cam1width)
#        camera1.set(4,cam1height)

    if(mode==2 or mode == 0):
        camera2 = cv2.VideoCapture(1)
#        camera2.set(3,cam2width)
#        camera2.set(4,cam2height)
    
    while(True):
        # Capture frame-by-frame
        
        if(mode == 1 or mode == 0):
            ret, frameC1 = camera1.read()
            cv2.imshow('Camera1',frameC1)
        
        if(mode == 2 or mode == 0):
            ret, frameC2 = camera2.read()
            cv2.imshow('Camera2',frameC2)
            
        if(mode == 0):  
            pass
#            distx = joinImages.calculateOffset(frameC1,frameC2)
            #distx=10   
#            frameC1_2 = joinImages.joinImages(frameC1,frameC2,distx)
#            cv2.imshow('Both',frameC1_2)

#        
#        if cv2.waitKey(1) & 0xFF == ord('q'):
#            break
        
    # When everything done, release the capture
    if(mode == 1 or mode == 0):
        camera1.release()
    if(mode == 2 or mode == 0):
        camera2.release()
    
    cv2.destroyAllWindows() 
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    