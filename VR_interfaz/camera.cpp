#include "camera.h"

/* Function Camera
 * -------------------------------
 * built in constructor
*/
Camera::Camera()
{
    PylonInitialize();
}

/* Function Camera
 * -------------------------------
 * Overloaded constructor sets up the CInstantCamera and raises a
 * messagebox warning in case that the camera isn't connected or is busy
 *
 * num_cam : number of the camera selected
*/
Camera::Camera(int num_cam)
{
    PylonInitialize();
    CTlFactory& tlFactory = CTlFactory::GetInstance();
    DeviceInfoList_t devices;
    int num_cameras = tlFactory.EnumerateDevices(devices);

    if(num_cameras-1 >= num_cam)
    {
        this->m_pylon_camera = new CInstantCamera( CTlFactory::GetInstance().CreateDevice(devices[num_cam]));
        this->m_pylon_camera->Open();
    }
    else
    {
        QMessageBox Msgbox;
        Msgbox.setIcon(Msgbox.Critical);
        Msgbox.setText("<big>Warning</big> <p>\n\n Camera not conected \n The program won't work without both cameras conected</p>");
        Msgbox.exec();
    }
}

/* Function Camera
 * -------------------------------
 * built in destructor
*/
Camera::~Camera()
{
//    m_pylon_camera->Close();
    m_pylon_camera->DestroyDevice();
//    this->m_pylon_camera->DetachDevice();
    delete m_pylon_camera;
    PylonTerminate();
}

/* Function startGrabbing
 * -------------------------------
 * calls the a CInstantCamera function StartGrabbing
 *  Starts the grabbing of images from the camera
 *
 * grab_strategy : the grabbing strategy, see Pylon::EGrabStrategy,
 *                  by default GrabStrategy_LatestImageOnly used
*/
void Camera::startGrabbing(EGrabStrategy grab_strategy)
{
    if(!isGrabbing())
        this->m_pylon_camera->StartGrabbing(grab_strategy);
}

/* Function stopGrabbing
 * -------------------------------
 * calls the a CInstantCamera function StopGrabbing
 *  stop the grabbing of images from the camera
*/
void Camera::stopGrabbing()
{
    //this->m_pylon_camera->Close();
    this->m_pylon_camera->StopGrabbing();
}

/* Function isGrabbing
 * -------------------------------
 * returns true if the camera is grabbing images
*/
bool Camera::isGrabbing()
{
    return this->m_pylon_camera->IsGrabbing();
}

/* Function hasImage
 * -------------------------------
 * returns if there is an image to grab
 *
 * return : boolean indicating if there is a image to grab
*/
bool Camera::hasImage()
{
    return this->m_pylon_camera->GetGrabResultWaitObject().Wait(0);
}

/* Function getIsinitUndistort
 * -------------------------------
 * return the a boolean that indicates if the camera calibration is
 * initizelized
 *
 * return : current boolean value
*/
bool Camera::getIsinitUndistort()
{
    return m_calib.isInitUndistort();
}

/* Function setBinning
 * -------------------------------
 *  this function sets the camera capture binning.
 *  the binning value needs to be between 1 and 4.
 *
 *  1 value will use the maximum camera resolution
 *  2,3,4 value will devide the original resolution 2,3,4 times.
 *  otherwise the binning will be left as it is.
 *
 *  binningValueHorizontal : horizontal binning value.
 *
 *  binningValueVertical : vertical binning value.
*/
void Camera::setBinning(int binningValueHorizontal,int binningValueVertical)
{
    this->m_pylon_camera->Open();

    INodeMap& nodemap = m_pylon_camera->GetNodeMap();
    if(binningValueHorizontal>0 && binningValueHorizontal<=4)
        CIntegerPtr(nodemap.GetNode("BinningHorizontal"))->SetValue(binningValueHorizontal);
    if(binningValueVertical>0 && binningValueVertical<=4)
        CIntegerPtr(nodemap.GetNode("BinningVertical"))->SetValue(binningValueVertical);

}

/* Function getHBinning
 * -------------------------------
 *  this function return the camera horizontal binning currently used
*/
int Camera::getHBinning()
{
    this->m_pylon_camera->Open();
    INodeMap& nodemap = m_pylon_camera->GetNodeMap();

    return CIntegerPtr(nodemap.GetNode("BinningHorizontal"))->GetValue();
}

/* Function getVBinning
 * -------------------------------
 *  this function return the camera horizontal binning currently used
*/
int Camera::getVBinning()
{
    this->m_pylon_camera->Open();
    INodeMap& nodemap = m_pylon_camera->GetNodeMap();

    return CIntegerPtr(nodemap.GetNode("BinningVertical"))->GetValue();
}

/* Function setflipYOutput
 * -------------------------------
 * sets a camera flag that will return the images flipped in the y axis.
 * The flip is set at the start to false by default
 *
 * flip : boolean that selects if the image will be flipped in the y axis,
 *        True -> the image will be flipped
 *        false -> the image won't be flipped
 *
*/
void Camera::setflipYOutput(bool flip)
{
    this->m_pylon_camera->Open();
    INodeMap& nodemap = m_pylon_camera->GetNodeMap();
    CBooleanPtr(nodemap.GetNode("ReverseY"))->SetValue(flip);
}

/* Function getflipY
 * -------------------------------
 * return true if the camera image is being flipped in the Y axis
*/
bool Camera::getflipY()
{
    this->m_pylon_camera->Open();
    INodeMap& nodemap = m_pylon_camera->GetNodeMap();
    return CBooleanPtr(nodemap.GetNode("ReverseY"))->GetValue();
}

/* Function setflipXOutput
 * -------------------------------
 * sets a camera flag that will return the images flipped in the x axis.
 * The flip is set at the start to false by default
 *
 * flip : boolean that selects if the image will be flipped in the x axis,
 *        True -> the image will be flipped
 *        false -> the image won't be flipped
 *
*/
void Camera::setflipXOutput(bool flip)
{
    this->m_pylon_camera->Open();
    INodeMap& nodemap = m_pylon_camera->GetNodeMap();
    CBooleanPtr(nodemap.GetNode("ReverseX"))->SetValue(flip);
}

/* Function getflipX
 * -------------------------------
 * return true if the camera image is being flipped in the x axis
 *
*/
bool Camera::getflipX()
{
    this->m_pylon_camera->Open();
    INodeMap& nodemap = m_pylon_camera->GetNodeMap();
    return CBooleanPtr(nodemap.GetNode("ReverseX"))->GetValue();
}


/* Function setResolution
 * -------------------------------
 * sets the camera image resolution
 *
 * width: desired width of the image must be lower or equal
 *        to the camera's max width
 * height: desired height of the image must be lower or equal
 *         to the camera's max height
*/
void Camera::setResolution(int width,int height)
{
    this->m_pylon_camera->Open();
    INodeMap& nodemap = m_pylon_camera->GetNodeMap();

    long maxWidth = CIntegerPtr(nodemap.GetNode("WidthMax"))->GetValue();
    long maxHeight = CIntegerPtr(nodemap.GetNode("HeightMax"))->GetValue();

    if(width>0 && width<=maxWidth)
        CIntegerPtr(nodemap.GetNode("Width"))->SetValue(odd2Even(width));
    else
        CIntegerPtr(nodemap.GetNode("Width"))->SetValue(odd2Even(maxWidth));

    if(height>0 && height<=maxHeight)
        CIntegerPtr(nodemap.GetNode("Height"))->SetValue(odd2Even(height));
    else
        CIntegerPtr(nodemap.GetNode("Height"))->SetValue(odd2Even(maxHeight));

}

/* Function setROIOffset
 * -------------------------------
 * sets the offset inside the sensor of the left up corner of the image
 *
*/
void Camera::setROIOffset(int x, int y)
{
    int width,height = 0;
    this->m_pylon_camera->Open();

    INodeMap& nodemap = m_pylon_camera->GetNodeMap();

    long maxWidth = CIntegerPtr(nodemap.GetNode("WidthMax"))->GetValue();
    long maxHeight = CIntegerPtr(nodemap.GetNode("HeightMax"))->GetValue();

    width = CIntegerPtr(nodemap.GetNode("Width"))->GetValue();
    height = CIntegerPtr(nodemap.GetNode("Height"))->GetValue();

    if(x>=0 && x<maxWidth && maxWidth>=x+width)
        CIntegerPtr(nodemap.GetNode("OffsetX"))->SetValue(odd2Even(x));
    if(y>=0 && y<maxHeight && maxHeight>=y+height)
        CIntegerPtr(nodemap.GetNode("OffsetY"))->SetValue(odd2Even(y));
}

/* Function getCurrentROIRect
 * -------------------------------
 * returns the current cv::rect where x and y are de offsets
 * of the image inside the sensor
 *
*/
Rect Camera::getCurrentROIRect()
{
    int width,height,x,y = 0;

    this->m_pylon_camera->Open();
    INodeMap& nodemap = m_pylon_camera->GetNodeMap();
    width = CIntegerPtr(nodemap.GetNode("Width"))->GetValue();
    height = CIntegerPtr(nodemap.GetNode("Height"))->GetValue();
    x = CIntegerPtr(nodemap.GetNode("OffsetX"))->GetValue();
    y = CIntegerPtr(nodemap.GetNode("OffsetY"))->GetValue();

    return Rect(x,y,width,height);
}

/* Function setROIRect
 * -------------------------------
 * sets the current camera roi rectangle
 *
*/
void Camera::setROIRect(Rect ROIrect)
{
    this->setResolution(ROIrect.width,ROIrect.height);
    this->setROIOffset(odd2Even(ROIrect.x),odd2Even(ROIrect.y));
}

/* Function getMaxWidth
 * -------------------------------
 * returns the maximum width avaliable in this camera
*/
int Camera::getMaxWidth()
{
    this->m_pylon_camera->Open();

    INodeMap& nodemap = m_pylon_camera->GetNodeMap();
    return CIntegerPtr(nodemap.GetNode("WidthMax"))->GetValue();
}

/* Function getMaxHeight
 * -------------------------------
 * returns the maximum height avaliable in this camera
*/
int Camera::getMaxHeight()
{
    this->m_pylon_camera->Open();

    INodeMap& nodemap = m_pylon_camera->GetNodeMap();
    return CIntegerPtr(nodemap.GetNode("HeightMax"))->GetValue();
}

/* Function moveROIBy
 * -------------------------------
 * this function moves the current ROI in the sensor
 *
 * x: movement done in the x axis
 * y: movement done in the y axis
*/
void Camera::moveROIBy(int x, int y)
{
    Rect rectangle = this->getCurrentROIRect();

    this->setROIOffset(odd2Even(rectangle.x+x),odd2Even(rectangle.y+y));
}

/* Function grab_image
 * -------------------------------
 * grabs a image from the CInstantCamera
 *
 * &ret : boolean that indicates if the grabbing has been successfully.
 *
 * return -> image grabbed in QImage type
*/
QImage* Camera::grab_image(bool &ret)
{
    ret = false;
    QImage *qImage = NULL;
    CPylonImage image;
    CImageFormatConverter fc;
    fc.OutputPixelFormat = PixelType_RGB8packed; 
    CGrabResultPtr ptrGrabResult;

    this->m_pylon_camera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
    if (ptrGrabResult->GrabSucceeded())
    {
        ret = true;

        fc.Convert(image, ptrGrabResult);
        size_t bufferSize = image.GetAllocatedBufferSize();
        uint8_t *nBuffer = new uint8_t[bufferSize];
        memcpy(nBuffer,image.GetBuffer(),bufferSize);
        qImage = new QImage((uint8_t *)nBuffer, ptrGrabResult->GetWidth(), ptrGrabResult->GetHeight(),QImage::Format_RGB888);
    }
    return qImage;
}

/* Function grab_imageMemSafe
 * -------------------------------
 * callback used when a QImage returned by the memsafe functions is deleted
 *
 * info : as a part of the callback, this variable contains the pointer to the data buffer
 *
 * return -> image grabbed in QImage type wrapped on a shared_ptr
*/
void cleanupBufferImage(void* info)
{
    uint8_t* temp = (uint8_t*) info;
    delete[] temp;
}

/* Function grab_imageMemSafe
 * -------------------------------
 * Memory safe version of the grab_image function
 *
 * &ret : boolean that indicates if the grabbing has been successfully.
 *
 * return -> image grabbed in QImage type wrapped on a shared_ptr
*/
std::shared_ptr<QImage> Camera::grab_imageMemSafe(bool &ret)
{
    ret = false;
    std::shared_ptr<QImage> qImage = std::shared_ptr<QImage>(nullptr);
    CPylonImage image;
    CImageFormatConverter fc;
    fc.OutputPixelFormat = PixelType_RGB8packed;
    CGrabResultPtr ptrGrabResult;

    this->m_pylon_camera->RetrieveResult( 5000, ptrGrabResult, TimeoutHandling_ThrowException);
    if (ptrGrabResult->GrabSucceeded())
    {
        ret = true;
        fc.Convert(image, ptrGrabResult);
        size_t bufferSize = image.GetAllocatedBufferSize();
        uint8_t *nBuffer = new uint8_t[bufferSize];
        memcpy(nBuffer,image.GetBuffer(),bufferSize);
        qImage.reset(new QImage((uint8_t *)nBuffer, ptrGrabResult->GetWidth(), ptrGrabResult->GetHeight(),QImage::Format_RGB888,cleanupBufferImage,nBuffer));
    }
    return qImage;
}

/* Function single_grab_image
 * -------------------------------
 * open the camera grabs an image and closes it
 *
 * &ret : boolean that indicates if the grabbing has been successfully.
 *
 * return -> image grabbed in QImage type
*/
QImage* Camera::single_grab_image(bool &ret)
{
    this->startGrabbing();
    QImage* image = grab_image(ret);
    this->stopGrabbing();
    return image;
}

/* Function single_grab_imageMemSafe
 * -------------------------------
 * Memsafe version of the single_grab_image
 *
 * &ret : boolean that indicates if the grabbing has been successfully.
 *
 * return -> image grabbed in QImage type wrapped on a shared_ptr
*/
std::shared_ptr<QImage> Camera::single_grab_imageMemSafe(bool &ret)
{
    this->startGrabbing();
    std::shared_ptr<QImage> image = grab_imageMemSafe(ret);
    this->stopGrabbing();
    return image;
}

/* Function initCamParametersFromYALM
 * -------------------------------
 * loads the camera configuration using a YAML file
 * will raise a messagebox warning if the maximum resolution from the file
 * isn't equal to the maximum resolution of the running camera
 *
 * filename : YAML file path that contains the camera configuration
*/
void Camera::initCamParametersFromYALM(QString filename)
{
    this->stopGrabbing();
    int hBinning,vBinning,width,height,xOffset,yOffset,maxHeight,maxWidth = 0;
    bool xflip,yflip = false;

    FileStorage fs(filename.toStdString(), FileStorage::READ);
    fs["horizontalBinning"] >> hBinning;
    fs["verticalBinning"] >> vBinning;
    fs["xflip"] >> xflip;
    fs["yflip"] >> yflip;
    fs["maxWidth"] >> maxWidth;
    fs["maxHeight"] >> maxHeight;
    fs["width"] >> width;
    fs["height"] >> height;
    fs["xOffset"] >> xOffset;
    fs["yOffset"] >> yOffset;

    if(maxWidth != this->getMaxWidth() || maxHeight != this->getMaxHeight())
    {
        QMessageBox Msgbox;
        Msgbox.setIcon(Msgbox.Warning);
        Msgbox.setText("<big>Warning</big> <p>\n\n Camera Resolution aren't equal \n It's posible that the YALM file is from other camera</p>");
        Msgbox.exec();
    }
    this->setBinning(hBinning,vBinning);
    this->setResolution(width,height);
    this->setROIOffset(xOffset,yOffset);
    this->setflipXOutput(xflip);
    this->setflipYOutput(yflip);
}

/* Function initCalibParams
 * -------------------------------
 * function used to initialize the calibration params from two files.
 *
 * intrinsicFilename : filepath of the instrinsic matrix in csv format
 * distCoeffsFilename : filepath of the distortion coeficient in csv format
 *
 * return -> boolean indicates if the calibration paramenters have been loaded
 *          successfully.
*/
bool Camera::initCalibParams(QString calibConfigFile)
{
    m_calib.calibrateFromFile(calibConfigFile.toLatin1().data());
    return m_calib.isCalibrated();
}

/* Function initUndistortMap
 * -------------------------------
 * function used to initialize and map the calibration params
 *
 * imageSize : width and height of the image that will be transformed
*/
void Camera::initUndistortMap(Size imageSize)
{
    m_calib.initUndistortImage(imageSize);
}

/* Function undistortMapImage
 * -------------------------------
 * function used to undistort an image using mapping
 *
 * src : image used to perform the undistortion
 * interpolation : interpolation used in the remap function, see cv::remap
 *
 * return -> the input image undistorted
*/
QImage Camera::undistortMapImage(QImage src, int interpolation)
{
    return Mat2QImage(m_calib.undistort(QImage2Mat(src),interpolation));
}

CameraCalibration Camera::getCalibration()
{
    return m_calib;
}

void Camera::setCalibration(CameraCalibration calib)
{
    m_calib = calib;
}

bool Camera::isCalibrated()
{
    return m_calib.isCalibrated();
}

bool Camera::isCameraConnected()
{
    return !this->m_pylon_camera->IsCameraDeviceRemoved() && this->m_pylon_camera->IsOpen();
}

bool Camera::reconnect(int CamNum)
{
    bool hasReconnected = false;
    PylonInitialize();
    CTlFactory& tlFactory = CTlFactory::GetInstance();
    DeviceInfoList_t devices;
    int num_cameras = tlFactory.EnumerateDevices(devices);

    if(num_cameras-1 >= CamNum)
    {
        this->m_pylon_camera = new CInstantCamera( CTlFactory::GetInstance().CreateDevice(devices[CamNum]));
        this->m_pylon_camera->Open();
        hasReconnected = true;
    }
    return hasReconnected;
}

