# -*- coding: utf-8 -*-
"""
Created on Tue Jul 04 10:33:38 2017

@author: Hp
"""

import numpy as np
import cv2
import matplotlib.pyplot as plt
from skimage import filters,transform,feature
from scipy import ndimage
from scipy import signal 

def calculateOffset(im1,im2):
    """
    This function calculates the affine offset in de x axis between the two 
    images 
    im1,im2 -> image matrix in uint8 format
    
    return integer with the offset
    """
       
    im1 = np.array(cv2.cvtColor(im1, cv2.COLOR_BGR2GRAY),dtype="float64")
    im2 = np.array(cv2.cvtColor(im2, cv2.COLOR_BGR2GRAY),dtype="float64")
    
#    im1 = filters.gaussian(im1,5)
#    im2 = filters.gaussian(im2,5)
        
#    plt.figure(), plt.imshow(im1,cmap=plt.cm.gray)
#    plt.figure(), plt.imshow(im2,cmap=plt.cm.gray)

    im1 = np.array(feature.canny(im1,5),dtype="float64")
    im2 = np.array(feature.canny(im2,5),dtype="float64")
    
    plt.figure(), plt.imshow(im1,cmap=plt.cm.gray)
    plt.figure(), plt.imshow(im2,cmap=plt.cm.gray)

    im1 = transform.rotate(im1,180)
    
    result = signal.fftconvolve(im1, im2)
    
    plt.figure(), plt.imshow(result,cmap=plt.cm.gray)

    __,__,__,coordinates = cv2.minMaxLoc(result)   
    
    return result.shape[1]-coordinates[0]
#    return 0

def joinImages(im1,im2,offsetX,mode="mean"):
    """
    im1,im2 -> image matrix in uint8 format
    
    offsetX -> integer, contains the diference between the two images
    
    mode -> · concat -> concatenates the images in the x axis 
    
            · offsetcut -> concatenate the images and removes the area of the 
            image contained inside the offset

            · mean -> concatenates the images and calculates the mean between 
            the two areas overlapped

            · progresive -> concatenates the images and calculates the Weighted
            mean between the two areas overlapped
    
    return a image matrix in uint8 composed of the given images     
    """
    (im1_height, im1_width ,channel) = im1.shape
    (im2_height, im2_width ,__) = im2.shape
    
    height = im1_height
    if(im1_height < im2_height): 
        height = im2_height
    
    if(mode == "concat" or offsetX == 0):
        imjoin = np.zeros((height,im1_width+im2_width,channel),dtype="uint8")
        
        imjoin[0:im1_height,0:im1_width,:] = im1[:,:,:]
        imjoin[0:im2_height, im1_width:im1_width+im2_width ,:] = im2[:,:,:]
    
    elif(mode == "offsetcut"): 
    
        imjoin = np.zeros((height,im1_width+im2_width-2*offsetX,channel),dtype="uint8")
        
        imjoin[0:im1_height,0:im1_width-offsetX,:] = im1[:,:-offsetX,:]
        imjoin[0:im2_height, im1_width-offsetX:im1_width+im2_width ,:] = im2[:,offsetX:,:]
        
    elif(mode == "mean"):
        
        imjoin = np.zeros((height,im1_width+im2_width-2*offsetX,channel),dtype="float64")
        
        imjoin[0:im1_height,0:im1_width-offsetX,:] = im1[:,:-offsetX,:]
        imjoin[0:im2_height, im1_width-offsetX:im1_width+im2_width ,:] = im2[:,offsetX:,:]
        
        cutArea1 = im1[:im1_height, im1_width-2*offsetX:im1_width ,:]
        cutArea2 = im2[:im2_height, 0:2*offsetX,:]
        
        intersectedArea = np.mean([cutArea1,cutArea2],axis=0);
        
        imjoin[:,im1_width-2*offsetX:im1_width,:] = intersectedArea
        
        imjoin = np.array(imjoin,dtype="uint8")
        
        
    elif(mode =="progressive"):
        
        imjoin = np.zeros((height,im1_width+im2_width-2*offsetX,channel),dtype="float64")
        
        imjoin[0:im1_height,0:im1_width-offsetX,:] = im1[:,:-offsetX,:]
        imjoin[0:im2_height, im1_width-offsetX:im1_width+im2_width ,:] = im2[:,offsetX:,:]
        
        cutArea1 = im1[:im1_height, im1_width-2*offsetX:im1_width ,:]
        cutArea2 = im2[:im2_height, 0:2*offsetX,:]
        
        
        stepRef=1.0/cutArea1.shape[1]
        step = stepRef
        for i in reversed(range(0,cutArea1.shape[1])):
            cutArea1[:,i,:] = cutArea1[:,i,:]*step
            step += stepRef
        
        step = stepRef
        for i in range(0,cutArea2.shape[1]):
            cutArea2[:,i,:] = cutArea2[:,i,:]*step
            step += stepRef
            
        intersectedArea = cutArea1+cutArea2
        
        imjoin[:,im1_width-2*offsetX:im1_width,:] = intersectedArea
        
        imjoin = np.array(imjoin,dtype="uint8")
    
    return imjoin

