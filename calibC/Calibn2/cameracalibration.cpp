#include "cameracalibration.h"

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

Mat CameraCalibration::getDistorsionMatrix()
{
    return m_distorsionMatrix;
}

Mat CameraCalibration::getIntrinsicMatrix()
{
    return m_intrinsicMatrix;
}

void CameraCalibration::calibrateFromFile(char *configFileName)
{
    m_isCalibrated = false;

    FileStorage fs(configFileName, FileStorage::READ);
    fs["IntrinsicMat"] >> m_intrinsicMatrix;
    fs["DistorsionVector"] >> m_distorsionVector;
    fs["boardWidth"] >> m_boardWidth;
    fs["boardHeight"] >> m_boardHeight;
    fs["squareSize"] >> m_squareSize;
    fs["calibrationError"] >> m_calibrationError;

    cout <<  "Calibration error: " << m_calibrationError << "\n";

    if(m_intrinsicMatrix != NULL && m_distorsionVector != NULL)
        m_isCalibrated = true;
}

void CameraCalibration::calibrateFromImages(int boardWidth, int boardHeight, int numImgs, float squareSize,
                                            char *imgFilePath, char *imgsFilename, char *imgExtension)
{
    vector< Mat > rvecs, tvecs;
    m_isCalibrated = false;
    loadFromImagesPoints(boardWidth, boardHeight, numImgs, squareSize,
                         imgFilePath, imgsFilename, imgExtension);

    printf("Starting Calibration\n");
    int flag = 0;
    flag |= CV_CALIB_FIX_K4;
    flag |= CV_CALIB_FIX_K5;
    calibrateCamera(m_objectPoints, m_imagePoints, m_imageSize, m_intrinsicMatrix, m_distorsionVector, rvecs,
                    tvecs, flag);

    cout << "Calibration error: " << computeReprojectionErrors(rvecs, tvecs) << "\n";

    if(m_intrinsicMatrix != NULL && m_distorsionVector != NULL)
        m_isCalibrated = true;
}

void CameraCalibration::loadFromImagesPoints(int boardWidth, int boardHeight, int numImgs,float squareSize,
                                             char* imgFilePath, char* imgsFilename, char* imgExtension)
{
    Mat img, gray;
    vector< Point2f > corners;
    Size boardSize = Size(boardWidth, boardHeight);

    for (int k = 1; k <= numImgs; k++) {
      char imgFile[100];
      sprintf(imgFile, "%s%s%d.%s", imgFilePath, imgsFilename, k, imgExtension);
      cout << imgFile <<" ";
      img = imread(imgFile, CV_LOAD_IMAGE_COLOR);
      m_imageSize = img.size();

      cvtColor(img, gray, CV_BGR2GRAY);

      bool found = false;
      found = cv::findChessboardCorners(img, boardSize, corners,
                                        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
      if (found)
      {
        cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1),
                     TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
        drawChessboardCorners(gray, boardSize, corners, found);
      }

      vector< Point3f > obj;
      for (int i = 0; i < board_height; i++)
        for (int j = 0; j < board_width; j++)
          obj.push_back(Point3f((float)j * squareSize, (float)i * square_size, 0));

      if (found) {
        cout << k << ". Found corners!" << "\n";
        m_imagePoints.push_back(corners);
        m_objectPoints.push_back(obj);
      } else {
        cout << "WARNING!! Corners not found!" << "\n";
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

bool saveParamsInFile(char* configFileName)
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

        saved = true;
    }
    return saved;
}
