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

    void calibrateFromFile(char* configFileName);
    void calibrateFromImages(int boardWidth, int boardHeight, int numImgs,float squareSize,
                             char* imgFilePath, char* imgsFilename, char* imgExtension);

    bool isCalibrated();

    Mat getIntrinsicMatrix();
    Mat getDistorsionVector();

    bool saveParamsInFile(char* configFileName);

private:
    void loadFromImagesPoints(int boardWidth, int boardHeight, int numImgs,float squareSize,
                              char* imgFilePath, char* imgsFilename, char* imgExtension);
    double computeReprojectionErrors(const vector< Mat >& rvecs, const vector< Mat >& tvecs);

    bool m_isCalibrated;
    Mat m_intrinsicMatrix;
    Mat m_distorsionVector;

    int m_boardWidth;
    int m_boardHeight;
    int m_squareSize;
    double m_calibrationError;

    vector< vector< Point3f > > m_objectPoints;
    vector< vector< Point2f > > m_imagePoints;
    Mat::size m_imageSize;
};

#endif // CAMERACALIBRATION_H
