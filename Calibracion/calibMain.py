# -*- coding: utf-8 -*-
"""
Created on Wed Jul 19 15:57:31 2017

@author: Hp
"""

import calibrateCamera as cc
reload(cc)

def main():
    cameraL = cc.calibrateCamera()
    cameraL.calibrateCamera((11,7),"./Camera Left")
    
    cameraR = cc.calibrateCamera()
    cameraR.calibrateCamera((11,7),"./Camera Right")
    
    cc.saveNumpyMatrix("cameraLmatrix.txt",cameraL.matrix)
    cc.saveNumpyMatrix("cameraLdist.txt",cameraL.dist)
    cc.saveNumpyMatrix("cameraRmatrix.txt",cameraR.matrix)
    cc.saveNumpyMatrix("cameraRdist.txt",cameraR.dist)


if __name__ == "__main__":
    main()