#ifndef STEREOCALIBRATION_H
#define STEREOCALIBRATION_H

#include "cameracalibration.h"

class StereoCalibration
{
public:
    StereoCalibration(CameraCalibration camLeft, CameraCalibration CamRight, int boardWidth, int boardHeight,
                      int numImgs, float squareSize, char* leftImgDir, char* rightImgDir,
                      char* leftImgFilename, char* rightImgFilename, char* extension);

    StereoCalibration(CameraCalibration camLeft, CameraCalibration CamRight, char* stereoConfig);

    void calibrateStereoFromImage(CameraCalibration camLeft, CameraCalibration CamRight, int boardWidth, int boardHeight,
                                  int numImgs, float squareSize, char* leftImgDir, char* rightImgDir,
                                  char* leftImgFilename, char* rightImgFilename, char* extension);

    void calibrateStereoFromFile(CameraCalibration camLeft, CameraCalibration CamRight, char* stereoConfig);

    void initUndistortImage();
    void undistort(Mat imgLeftIn,Mat imgRightIn, Mat imgLeftOut, Mat imgRightOut);

    bool saveParamsInFile(char* configFileName);

    bool isCalibrated();
    bool isInitUndistort();
private:
    void loadFromImagesPoints(int boardWidth, int boardHeight, int numImgs, float squareSize,
                              char* leftImgDir, char* rightImgDir, char* leftImgFilename,
                              char* rightImgFilename, char* extension);

    CameraCalibration m_camRight;
    CameraCalibration m_camLeft;

    bool m_isCalibrated;
    bool m_isInitUndistort;

    Mat m_R;
    Vec3d m_T;
    Mat m_E;
    Mat m_F;

    Mat m_R1;
    Mat m_R2;

    Mat m_P1;
    Mat m_P2;

    Mat m_Q;

    vector< vector<Point3f> > m_objectPoints;
    vector< vector<Point2f> > m_leftImagePoints;
    vector< vector<Point2f> > m_rightImagePoints;
    Size m_imageSize;
};

#endif // STEREOCALIBRATION_H
