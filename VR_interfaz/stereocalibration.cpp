#include "stereocalibration.h"
#include <QDebug>

StereoCalibration::StereoCalibration()
{
    m_isCalibrated = false;
    m_isInitUndistort = false;
}

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

void StereoCalibration::calibrateStereoFromImage(CameraCalibration camLeft, CameraCalibration camRight,
                                                 int boardWidth, int boardHeight, int numImgs, float squareSize,
                                                 char *leftImgDir, char *rightImgDir, char *leftImgFilename,
                                                 char *rightImgFilename, char *extension)
{
    m_isCalibrated = false;
    m_camLeft = camLeft;
    m_camRight = camRight;
    m_boardHeight = boardHeight;
    m_boardWidth = boardWidth;
    m_squareSize = squareSize;

    m_R = Mat();
    m_T = Vec3d();
    m_E = Mat();
    m_F = Mat();
    m_R1 = Mat();
    m_R2 = Mat();
    m_P1 = Mat();
    m_P2 = Mat();
    m_Q = Mat();
    m_lMapX = Mat();
    m_lMapY = Mat();
    m_rMapX = Mat();
    m_rMapY = Mat();

    m_objectPoints.clear();
    m_leftImagePoints.clear();
    m_rightImagePoints.clear();

    loadFromImagesPoints(numImgs, leftImgDir, rightImgDir,leftImgFilename, rightImgFilename, extension);

    int flagCalib = 0;
    int flag = 0;
    flagCalib |= CALIB_ZERO_TANGENT_DIST;
    flag |= CV_CALIB_ZERO_DISPARITY ;
    flagCalib |= CV_CALIB_FIX_K3;
    flagCalib |= CV_CALIB_FIX_K4;
    flagCalib |= CV_CALIB_FIX_K5;
    flagCalib |= CV_CALIB_FIX_K6;
//    flagCalib |= CALIB_USE_INTRINSIC_GUESS;
    flagCalib |= CALIB_FIX_INTRINSIC;
    flagCalib |= CALIB_FIX_PRINCIPAL_POINT  ;
    flagCalib |= CALIB_FIX_ASPECT_RATIO  ;

    Mat tempIntrisicL = m_camLeft.getIntrinsicMatrix();
    Mat tempIntrisicR = m_camRight.getIntrinsicMatrix();
    Mat tempDistL = m_camLeft.getDistorsionVector();
    Mat tempDistR = m_camRight.getDistorsionVector();

    stereoCalibrate(m_objectPoints, m_leftImagePoints, m_rightImagePoints,
                    tempIntrisicL, tempDistL, tempIntrisicR, tempDistR,
                    m_imageSize, m_R, m_T, m_E, m_F,flagCalib);

    stereoRectify(tempIntrisicL, tempDistL,
                  tempIntrisicR, tempDistR, m_imageSize, m_R, m_T, m_R1,
                  m_R2, m_P1, m_P2, m_Q, flag,0);

    m_camLeft.setIntrinsicMatrix(tempIntrisicL);
    m_camRight.setIntrinsicMatrix(tempIntrisicR);

    m_camLeft.setDistorsionVector(tempDistL);
    m_camRight.setDistorsionVector(tempDistR);

    m_isCalibrated = true;
}

void StereoCalibration::calibrateStereoFromFile(CameraCalibration camLeft, CameraCalibration CamRight, char *stereoConfig)
{
    m_isCalibrated = false;

    m_camLeft = camLeft;
    m_camRight = CamRight;

    m_R = Mat();
    m_T = Vec3d();
    m_E = Mat();
    m_F = Mat();
    m_R1 = Mat();
    m_R2 = Mat();
    m_P1 = Mat();
    m_P2 = Mat();
    m_Q = Mat();
    m_lMapX = Mat();
    m_lMapY = Mat();
    m_rMapX = Mat();
    m_rMapY = Mat();

    FileStorage fs(stereoConfig, FileStorage::READ);
    fs["R"] >> m_R;
    fs["T"] >> m_T;
    fs["E"] >> m_E;
    fs["F"] >> m_F;
    fs["R1"] >> m_R1;
    fs["R2"] >> m_R2;
    fs["P1"] >> m_P1;
    fs["P2"] >> m_P2;
    fs["Q"] >> m_Q;

    fs["boardWidth"] >> m_boardWidth;
    fs["boardHeight"] >> m_boardHeight;
    fs["squareSize"] >> m_squareSize;
    fs["imageSize"] >> m_imageSize;

    m_isCalibrated = true;
}

void StereoCalibration::loadFromImagesPoints(int numImgs, char *leftImgDir, char *rightImgDir,
                                             char *leftImgFilename, char *rightImgFilename, char *extension)
{
    Mat img1,img2, gray1, gray2;

    vector< Point2f > corners1, corners2;
    vector< vector< Point2f > > imagePoints1, imagePoints2;

    Size boardSize = Size(m_boardWidth, m_boardHeight);

    for (int i = 1; i <= numImgs; i++) {
      char left_img[100], right_img[100];
      sprintf(left_img, "%s/%s%d.%s", leftImgDir, leftImgFilename, i, extension);
      sprintf(right_img, "%s/%s%d.%s", rightImgDir, rightImgFilename, i, extension);

      img1 = imread(left_img, CV_LOAD_IMAGE_COLOR);
      img2 = imread(right_img, CV_LOAD_IMAGE_COLOR);
      m_imageSize = img1.size();
      cvtColor(img1, gray1, CV_BGR2GRAY);
      cvtColor(img2, gray2, CV_BGR2GRAY);

      bool found1 = false, found2 = false;

      found1 = cv::findChessboardCorners(img1, boardSize, corners1,
    CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
      found2 = cv::findChessboardCorners(img2, boardSize, corners2,
    CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

      if (found1)
      {
        cv::cornerSubPix(gray1, corners1, cv::Size(5, 5), cv::Size(-1, -1),
    cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 100, 0.01));
        cv::drawChessboardCorners(gray1, boardSize, corners1, found1);
      }
      if (found2)
      {
        cv::cornerSubPix(gray2, corners2, cv::Size(5, 5), cv::Size(-1, -1),
    cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 100, 0.01));
        cv::drawChessboardCorners(gray2, boardSize, corners2, found2);
      }

      vector< Point3f > obj;
      for (int i = 0; i < m_boardHeight; i++)
        for (int j = 0; j < m_boardWidth; j++)
          obj.push_back(Point3f((float)j * m_squareSize, (float)i * m_squareSize, 0));

      if (found1 && found2) {
        qDebug() << "Image:" << i << "Both corners found!";
        imagePoints1.push_back(corners1);
        imagePoints2.push_back(corners2);
        m_objectPoints.push_back(obj);
      } else {
        qDebug() << "Image:" << i << "WARNING Corners not found, Corner1:" << found1 << "Corner2:" << found2;
      }
    }

    for (int i = 0; i < imagePoints1.size(); i++) {
      vector< Point2f > v1, v2;
      for (int j = 0; j < imagePoints1[i].size(); j++) {
        v1.push_back(Point2f((double)imagePoints1[i][j].x, (double)imagePoints1[i][j].y));
        v2.push_back(Point2f((double)imagePoints2[i][j].x, (double)imagePoints2[i][j].y));
      }
      m_leftImagePoints.push_back(v1);
      m_rightImagePoints.push_back(v2);
    }
}

bool StereoCalibration::saveParamsInFile(char* configFileName)
{
    bool saved = false;
    if(isCalibrated()) {
        FileStorage fs(configFileName, FileStorage::WRITE);

        fs << "R" << m_R;
        fs << "T" << m_T;
        fs << "E" << m_E;
        fs << "F" << m_F;
        fs << "R1" << m_R1;
        fs << "R2" << m_R2;
        fs << "P1" << m_P1;
        fs << "P2" << m_P2;
        fs << "Q" << m_Q;

        fs << "boardWidth" << m_boardWidth;
        fs << "boardHeight" << m_boardHeight;
        fs << "squareSize" << m_squareSize;
        fs << "imageSize" << m_imageSize;

        saved = true;
    }
    return saved;
}

void StereoCalibration::initUndistortImage()
{
    this->initUndistortImage(m_imageSize);
}

void StereoCalibration::initUndistortImage(Size imageSize)
{
    m_isInitUndistort = false;

    cv::initUndistortRectifyMap(m_camLeft.getIntrinsicMatrix(), m_camLeft.getDistorsionVector(),
                                m_R1, m_P1, imageSize, CV_32F, m_lMapX, m_lMapY);
    cv::initUndistortRectifyMap(m_camRight.getIntrinsicMatrix(), m_camRight.getDistorsionVector(),
                                m_R2, m_P2, imageSize, CV_32F, m_rMapX, m_rMapY);
    m_isInitUndistort = true;
}

Mat StereoCalibration::undistortLeft(Mat imgLeftIn, int interpolation)
{
    Mat imgOut;
    if(this->isCalibrated() && this->isInitUndistort())
        cv::remap(imgLeftIn, imgOut, m_lMapX, m_lMapY, interpolation);
    return imgOut;
}

Mat StereoCalibration::undistortRight(Mat imgRightIn, int interpolation)
{
    Mat imgOut;
    if(this->isCalibrated() && this->isInitUndistort())
        cv::remap(imgRightIn, imgOut, m_rMapX, m_rMapY, interpolation);
    return imgOut;
}

bool StereoCalibration::isCalibrated()
{
    return m_isCalibrated;
}

bool StereoCalibration::isInitUndistort()
{
    return m_isInitUndistort;
}


