#include "cameracalibration.h"
#include "QDebug"

CameraCalibration::CameraCalibration()
{
    m_isInitUndistort = false;
}

CameraCalibration::CameraCalibration(int boardWidth, int boardHeight, int numImgs,float squareSize,
                                     char* imgFilePath, char* imgsFilename, char* imgExtension)
{
    calibrateFromImages(boardWidth, boardHeight, numImgs, squareSize,
                                 imgFilePath, imgsFilename, imgExtension);
}

CameraCalibration::CameraCalibration(char* configFileName)
{
    calibrateFromFile(configFileName);
}

bool CameraCalibration::isCalibrated()
{
    return m_isCalibrated;
}

bool CameraCalibration::isInitUndistort()
{
    return m_isInitUndistort;
}

Mat CameraCalibration::getDistorsionVector()
{
    return m_distorsionVector;
}

void CameraCalibration::setDistorsionVector(Mat newVect)
{
    m_distorsionVector = newVect;
}

Mat CameraCalibration::getIntrinsicMatrix()
{
    return m_intrinsicMatrix;
}

void CameraCalibration::setIntrinsicMatrix(Mat newMatrix)
{
    m_intrinsicMatrix = newMatrix;
}

void CameraCalibration::calibrateFromFile(char *configFileName)
{
    m_isCalibrated = false;
    m_isInitUndistort = false;

    FileStorage fs(configFileName, FileStorage::READ);
    fs["IntrinsicMat"] >> m_intrinsicMatrix;
    fs["DistorsionVector"] >> m_distorsionVector;
    fs["boardWidth"] >> m_boardWidth;
    fs["boardHeight"] >> m_boardHeight;
    fs["squareSize"] >> m_squareSize;
    fs["calibrationError"] >> m_calibrationError;
    fs["imageSize"] >> m_imageSize;

    if(m_intrinsicMatrix.data != NULL && m_distorsionVector.data != NULL)
        m_isCalibrated = true;
}

void CameraCalibration::calibrateFromImages(int boardWidth, int boardHeight, int numImgs, float squareSize,
                                            char *imgFilePath, char *imgsFilename, char *imgExtension)
{
    m_isCalibrated = false;
    m_isInitUndistort = false;
    m_boardWidth = boardWidth;
    m_boardHeight = boardHeight;
    m_squareSize = squareSize;

    m_mapx = Mat();
    m_mapy = Mat();
    m_intrinsicMatrix = Mat();
    m_distorsionVector = Mat();

    m_imagePoints.clear();
    m_objectPoints.clear();

    vector< Mat > rvecs, tvecs;

    loadFromImagesPoints(numImgs, imgFilePath, imgsFilename, imgExtension);

//    printf("Starting Calibration\n");
    int flag = 0;
    flag |= CV_CALIB_FIX_K3;
    flag |= CV_CALIB_FIX_K4;
    flag |= CV_CALIB_FIX_K5;
    flag |= CV_CALIB_FIX_K6;
    flag |= CALIB_ZERO_TANGENT_DIST;
//    flag |= CALIB_FIX_ASPECT_RATIO ;
//    flag |= CALIB_FIX_PRINCIPAL_POINT;
    calibrateCamera(m_objectPoints, m_imagePoints, m_imageSize, m_intrinsicMatrix, m_distorsionVector, rvecs,
                    tvecs, flag);

    qDebug() << "Calibration error: " << computeReprojectionErrors(rvecs, tvecs);

    if(m_intrinsicMatrix.data != NULL && m_distorsionVector.data != NULL)
        m_isCalibrated = true;
}

void CameraCalibration::loadFromImagesPoints(int numImgs, char* imgFilePath, char* imgsFilename,
                                             char* imgExtension)
{
    Mat img, gray;
    vector< Point2f > corners;
    Size boardSize = Size(m_boardWidth, m_boardHeight);

    for (int k = 1; k <= numImgs; k++) {
      char imgFile[100];
      sprintf(imgFile, "%s/%s%d.%s", imgFilePath, imgsFilename, k, imgExtension);
//      qDebug() << imgFile <<" ";
      img = imread(imgFile, CV_LOAD_IMAGE_COLOR);
      m_imageSize = img.size();

      cvtColor(img, gray, CV_BGR2GRAY);

      bool found = false;
      found = cv::findChessboardCorners(img, boardSize, corners,
                                        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
      if (found)
      {
        cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1),
                     TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 100, 0.01));
        drawChessboardCorners(gray, boardSize, corners, found);
      }

      vector< Point3f > obj;
      for (int i = 0; i < m_boardHeight; i++)
        for (int j = 0; j < m_boardWidth; j++)
          obj.push_back(Point3f((float)j * m_squareSize, (float)i * m_squareSize, 0));

      if (found) {
        qDebug() << k << ". Found corners!";
        m_imagePoints.push_back(corners);
        m_objectPoints.push_back(obj);
      } else {
        qDebug() << "WARNING!! Corners not found!";
      }
    }
}

double CameraCalibration::computeReprojectionErrors(const vector<Mat> &rvecs, const vector<Mat> &tvecs)
{
  vector< Point2f > imagePoints2;
  int i, totalPoints = 0;
  double totalErr = 0, err;
  vector< float > perViewErrors;
  perViewErrors.resize(m_objectPoints.size());

  for (i = 0; i < (int)m_objectPoints.size(); ++i) {
    projectPoints(Mat(m_objectPoints[i]), rvecs[i], tvecs[i], m_intrinsicMatrix,
                  m_distorsionVector, imagePoints2);
    err = norm(Mat(m_imagePoints[i]), Mat(imagePoints2), CV_L2);
    int n = (int)m_objectPoints[i].size();
    perViewErrors[i] = (float) std::sqrt(err*err/n);
    totalErr += err*err;
    totalPoints += n;
  }
  m_calibrationError = std::sqrt(totalErr/totalPoints);
  return m_calibrationError;
}

bool CameraCalibration::saveParamsInFile(char* configFileName)
{
    bool saved = false;
    if(isCalibrated()) {
        FileStorage fs(configFileName, FileStorage::WRITE);

        fs << "IntrinsicMat" << m_intrinsicMatrix;
        fs << "DistorsionVector" << m_distorsionVector;
        fs << "boardWidth" << m_boardWidth;
        fs << "boardHeight" << m_boardHeight;
        fs << "squareSize" << m_squareSize;
        fs << "calibrationError" << m_calibrationError;
        fs << "imageSize" << m_imageSize;

        saved = true;
    }
    return saved;
}

void CameraCalibration::initUndistortImage()
{
    initUndistortImage(m_imageSize);
}
void CameraCalibration::initUndistortImage(Size imageSize)
{
    if(this->isCalibrated())
    {
        m_isInitUndistort = false;
        Mat R;
        cv::initUndistortRectifyMap(m_intrinsicMatrix, m_distorsionVector, R, m_intrinsicMatrix,
                                    imageSize, CV_32F, m_mapx, m_mapy);
        m_isInitUndistort = true;
    }
}

Mat CameraCalibration::undistort(Mat imgIn, int interpolation)
{
    Mat imgOut;
    if(this->isInitUndistort())
        cv::remap(imgIn, imgOut, m_mapx, m_mapy, interpolation);
    return imgOut;
}





















