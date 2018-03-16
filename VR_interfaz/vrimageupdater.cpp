#include "vrimageupdater.h"

/* Function VRimageUpdater
 * -------------------------------
 * build-in cosntructor.
*/
VRimageUpdater::VRimageUpdater()
{
}

/* Function ~VRimageUpdater
 * -------------------------------
 * Overloaded constructor
*/
VRimageUpdater::VRimageUpdater(Camera* camera,QTimer* timer, bool mirrored, bool isUndistorted)
{
    this->m_timeTrigger = timer;
    this->m_mirrored = mirrored;
    this->m_isUndistorted = isUndistorted;
    this->m_camera = camera;
    this->m_crono.start();
}

/* Function ~VRimageUpdater
 * -------------------------------
 * build-in destructor.
*/
VRimageUpdater::~VRimageUpdater()
{
    this->m_timeTrigger->stop();
}

/* Private slot frameUpdateEvent
 * -------------------------------
 * function called by the timer, that will grab an image from the camera and
 * will save it as the most recent image.
 * It will also undistort this image if the flag isundistorted is true
*/
void VRimageUpdater::frameUpdateEvent2()
{
    //this->m_crono.restart();

    m_mutex.lock();

    bool ret;
    QImage *qImage;

    qImage = this->m_camera->grab_image(ret);

    if(ret)
    {
        //this->m_crono.restart();
        if(this->m_isUndistorted && this->m_camera->getIsinitUndistort())
        {
            // qDebug() << "Undistort";
            m_frame = this->m_camera->undistortMapImage(*qImage,CV_INTER_LINEAR);
        }
        else
        {
            // qDebug() << "No undistort";
            m_frame = qImage->copy(qImage->rect());
        }
        //qDebug() << this->m_crono.elapsed();

        delete[] qImage->bits();
        delete qImage;
        qImage = NULL;

        m_currentFps = 1000.0/(this->m_crono.restart());
        //qDebug() << this->m_crono.restart();
    }

    m_mutex.unlock();
}

/* public Function getCurrentFPS
 * -------------------------------
 * function that will return the current fps
*/
float VRimageUpdater::getCurrentFPS()
{
    return this->m_currentFps;
}

/* public Function getNextFrame
 * -------------------------------
 * function that will return the most recent image
*/
QImage VRimageUpdater::getNextFrame()
{
    QImage h;

    m_mutex.lock();

    // return this->m_frame;
    h = m_frame.copy();

    m_mutex.unlock();

    return h;
}

/* public Function setIsUndistorted
 * -------------------------------
 * function that will set the undistorted flag
 *
 * isUndistorted : True -> the image will be undistorted
 *                 False -> the image won't be undistorted
*/
void VRimageUpdater::setIsUndistorted(bool isUndistorted)
{
    this->m_isUndistorted = isUndistorted;
}

/* public slot setUpdatingEvent
 * -------------------------------
 * function that will start or stop the updating of the images
 *
 * isUndistorted : True -> the image will be undistorted
 *                 False -> the image won't be undistorted
*/
void VRimageUpdater::setUpdatingEvent(bool updating)
{
    if(updating)
    {
        this->m_camera->startGrabbing(GrabStrategy_LatestImageOnly);
        this->m_timeTrigger->start();
    }
    else
    {
        this->m_timeTrigger->stop();
        this->m_camera->stopGrabbing();
    }
}

/* public slot zoomIn
 * -------------------------------
 * function that will zoom in on the camera sensor
 *
 * zoom : Zoom quantity in the camera sensor
*/
void VRimageUpdater::zoomIn(float zoom)
{
    Rect newRoi =Rect(0,0,0,0);
    int maxWidth, maxHeight = 0;

    setUpdatingEvent(false);
    maxWidth = this->m_camera->getMaxWidth();
    maxHeight = this->m_camera->getMaxHeight();

    Rect oldRoi = this->m_camera->getCurrentROIRect();
    newRoi.width =oldRoi.width - zoom;
    newRoi.height =oldRoi.height - zoom;
    newRoi.x = maxWidth/2-newRoi.width/2;
    newRoi.y = maxHeight/2-newRoi.height/2;

    this->m_camera->setROIRect(newRoi);
    setUpdatingEvent(true);
}

/* public slot zoomOut
 * -------------------------------
 * function that will zoom in on the camera sensor
 *
 * zoom : Zoom quantity in the camera sensor
*/
void VRimageUpdater::zoomOut(float zoom)
{
    Rect newRoi =Rect(0,0,0,0);
    int maxWidth, maxHeight = 0;

    setUpdatingEvent(false);
    maxWidth = this->m_camera->getMaxWidth();
    maxHeight = this->m_camera->getMaxHeight();

    Rect oldRoi = this->m_camera->getCurrentROIRect();
    newRoi.width =oldRoi.width + zoom;
    newRoi.height =oldRoi.height + zoom;
    newRoi.x = maxWidth/2-newRoi.width/2;
    newRoi.y = maxHeight/2-newRoi.height/2;

    this->m_camera->setROIRect(newRoi);
    setUpdatingEvent(true);
}












