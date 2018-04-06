#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;


class CameraCalibration
{
public:
    CameraCalibration(int boardWidth, int boardHeight, int numImgs,float squareSize,
                      char* imgFilePath, char* imgsFilename, char* imgExtension);
    CameraCalibration(char* configFileName);

    void calibrateFromFile();
    void calibrateFromImages(int boardWidth, int boardHeight, int numImgs,float squareSize,
                             char* imgFilePath, char* imgsFilename, char* imgExtension);

    bool isCalibrated();
    Mat getIntrinsicMatrix();
    Mat getDistorsionMatrix();

    void saveParamsInFile();
private:
    void loadFromFile();
    void loadFromImages();
    double computeReprojectionErrors();

    bool m_isInitialized;
    Mat m_intrinsicMatrix;
    Mat m_distorsionMatrix;

    int m_boardWidth;
    int m_boardHeight;
    int m_squareSize;
};

#endif // CAMERACALIBRATION_H
