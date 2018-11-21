#ifndef CAMERA_H
#define CAMERA_H

#include "QImage"
#include "QDebug"
#include "QMessageBox"
#include "QTimer"
#include "QTime"
#include "QFile"
#include <QMutex>
#include <QImageWriter>
#include <pylon/PylonIncludes.h>
#include <GenICamFwd.h>
#include "utils.h"

#include "cameracalibration.h"


#include <pylon/usb/BaslerUsbInstantCamera.h>
typedef Pylon::CBaslerUsbInstantCamera Camera_t;
using namespace Basler_UsbCameraParams;
using namespace Pylon;

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace GENAPI_NAMESPACE; //To use the basler api
using namespace cv;               //To use opencv

void cleanupBufferImage(void* info); //used to delete image correctly when the QImage is destroyed

/* Class Camera
 * -------------------------------
 * This class is used to simplify the use of the basler camera
 * api. It's build to work with all the basler camera portfolio.
 * It uses the CalibrationCamera class to save its own
 * calibration
*/
class Camera
{
public:
    Camera();
    Camera(int num_cam);
    ~Camera();

    QImage* grab_image(bool &ret);
    QImage* single_grab_image(bool &ret);
    std::shared_ptr<QImage> grab_imageMemSafe(bool &ret);
    std::shared_ptr<QImage> single_grab_imageMemSafe(bool &ret);
    void startGrabbing(EGrabStrategy grab_strategy=GrabStrategy_LatestImageOnly);
    void stopGrabbing();
    bool hasImage();
    bool getIsinitUndistort();
    bool isCalibrated();
    bool isCameraConnected();
    bool reconnect(int CamNum);

    void setBinning(int binningValueHorizontal,int binningValueVertical);
    void setflipYOutput(bool flip);
    void setflipXOutput(bool flip);
    void setResolution(int width,int height);
    void setROIOffset(int x, int y);
    void setROIRect(Rect ROIrect);
    void moveROIBy(int x,int y);

    Rect getCurrentROIRect();
    int getHBinning();
    int getVBinning();
    int getMaxWidth();
    int getMaxHeight();
    bool getflipX();
    bool getflipY();
    bool isGrabbing();

    void initCamParametersFromYALM(QString filename);
    bool initCalibParams(QString calibConfigFile);

    CameraCalibration getCalibration();
    void setCalibration(CameraCalibration calib);

    void initUndistortMap(Size imageSize);
    QImage undistortMapImage(QImage src, int interpolation);

private:
    CameraCalibration m_calib;

    CInstantCamera* m_pylon_camera;

    QTime crono; //temporal used for debugging
};

#endif // CAMERA_H
