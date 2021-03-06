#ifndef STEREOCALIBRATION_H
#define STEREOCALIBRATION_H

#include "cameracalibration.h"

/* Class CameraCalibration
 * -------------------------------
 * This class allows to calibrate pinhole stereo cameras and after
 * the calibration it allows the user to undistort and rectify stereo images
 *
 * The parameters of the calibration can be saved into a file using saveParamsInFile function
*/
class StereoCalibration
{
public:
    StereoCalibration();
    StereoCalibration(CameraCalibration camLeft, CameraCalibration CamRight, int boardWidth, int boardHeight,
                      int numImgs, float squareSize, char* leftImgDir, char* rightImgDir,
                      char* leftImgFilename, char* rightImgFilename, char* extension);

    StereoCalibration(CameraCalibration camLeft, CameraCalibration CamRight, char* stereoConfig);

    void calibrateStereoFromImage(CameraCalibration camLeft, CameraCalibration camRight, int boardWidth, int boardHeight,
                                  int numImgs, float squareSize, char* leftImgDir, char* rightImgDir,
                                  char* leftImgFilename, char* rightImgFilename, char* extension);

    void calibrateStereoFromFile(CameraCalibration camLeft, CameraCalibration CamRight, char* stereoConfig);

    void initUndistortImage();
    void initUndistortImage(Size imageSize);
    Mat undistortLeft(Mat imgLeftIn, int interpolation);
    Mat undistortRight(Mat imgRightIn, int interpolation);

    bool saveParamsInFile(char* configFileName);

    bool isCalibrated();
    bool isInitUndistort();

private:
    void loadFromImagesPoints(int numImgs, char* leftImgDir, char* rightImgDir, char* leftImgFilename,
                              char* rightImgFilename, char* extension);

    CameraCalibration m_camRight;
    CameraCalibration m_camLeft;
    int m_boardWidth;
    int m_boardHeight;
    float m_squareSize;

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

    Mat m_lMapX, m_lMapY, m_rMapX, m_rMapY;
    vector< vector<Point3f> > m_objectPoints;
    vector< vector<Point2f> > m_leftImagePoints;
    vector< vector<Point2f> > m_rightImagePoints;
    Size m_imageSize;
};

#endif // STEREOCALIBRATION_H
