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

/* Class CameraCalibration
 * -------------------------------
 * This class allows to calibrate pinhole cameras and after
 * the calibration it allows the user to undistort images captured with the same camera
 *
 * The parameters of the calibration can be saved into a file using saveParamsInFile function
*/
class CameraCalibration
{
public:
    CameraCalibration();
    CameraCalibration(int boardWidth, int boardHeight, int numImgs,float squareSize,
                      char* imgFilePath, char* imgsFilename, char* imgExtension);
    CameraCalibration(char* configFileName);

    void calibrateFromFile(char* configFileName);
    void calibrateFromImages(int boardWidth, int boardHeight, int numImgs,float squareSize,
                             char* imgFilePath, char* imgsFilename, char* imgExtension);
    void initUndistortImage();
    void initUndistortImage(Size imageSize);
    Mat undistort(Mat imgIn, int interpolation);

    bool saveParamsInFile(char* configFileName);

    bool isCalibrated();
    bool isInitUndistort();

    Mat getIntrinsicMatrix();
    void setIntrinsicMatrix(Mat newMatrix);
    Mat getDistorsionVector();
    void setDistorsionVector(Mat newVect);
private:
    void loadFromImagesPoints(int numImgs,char* imgFilePath, char* imgsFilename, char* imgExtension);

    double computeReprojectionErrors(const vector< Mat >& rvecs, const vector< Mat >& tvecs);

    bool m_isCalibrated;
    bool m_isInitUndistort;

    Mat m_intrinsicMatrix;
    Mat m_distorsionVector;

    int m_boardWidth;
    int m_boardHeight;
    float m_squareSize;
    double m_calibrationError;

    vector< vector<Point3f> > m_objectPoints;
    vector< vector<Point2f> > m_imagePoints;
    Size m_imageSize;

    Mat m_mapx;
    Mat m_mapy;
};

#endif // CAMERACALIBRATION_H
