#include "stereocalibration.h"

StereoCalibration::StereoCalibration(CameraCalibration camLeft, CameraCalibration CamRight, int boardWidth, int boardHeight,
                                     int numImgs, float squareSize, char* leftImgDir, char* rightImgDir,
                                     char* leftImgFilename, char* rightImgFilename, char* extension)
{
    this->calibrateStereoFromImage(camLeft, CamRight, boardWidth, boardHeight, numImgs, squareSize,
                                  leftImgDir, rightImgDir, leftImgFilename, rightImgFilename, extension);
}

StereoCalibration::StereoCalibration(CameraCalibration camLeft, CameraCalibration CamRight, char* stereoConfig)
{
    this->calibrateStereoFromFile(camLeft, CamRight, stereoConfig);
}

void StereoCalibration::calibrateStereoFromImage(CameraCalibration camLeft, CameraCalibration CamRight,
                                                 int boardWidth, int boardHeight, int numImgs, float squareSize,
                                                 char *leftImgDir, char *rightImgDir, char *leftImgFilename,
                                                 char *rightImgFilename, char *extension)
{
    m_camLeft = camLeft;
    m_camRight = CamRight;

    loadFromImagesPoints(boardWidth, boardHeight, numImgs, squareSize, leftImgDir, rightImgDir,
                         leftImgFilename, rightImgFilename, extension);
}


