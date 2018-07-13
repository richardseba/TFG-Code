#include "vrimageupdater.h"

#include "processingimages.h"

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
VRimageUpdater::VRimageUpdater(Camera* camera, bool mirrored, bool isUndistorted)
{
    this->m_mirrored = mirrored;
    this->m_isUndistorted = isUndistorted;
    this->m_camera = camera;
    this->m_crono.start();

    m_timeTrigger.moveToThread(&m_currentThread);
    connect(&m_timeTrigger, SIGNAL (timeout()), this, SLOT (frameUpdateEvent()));
    this->moveToThread(&m_currentThread);
    m_currentThread.start();
}

/* Function ~VRimageUpdater
 * -------------------------------
 * build-in destructor.
*/
VRimageUpdater::~VRimageUpdater()
{
    if(m_timeTrigger.isActive())
    {
        this->waitUpdateFinished();
    }
    this->m_currentThread.quit();
    this->m_currentThread.wait();
}

/* Private slot frameUpdateEvent
 * -------------------------------
 * function called by the timer, that will grab an image from the camera and
 * will save it as the most recent image.
 * It will also undistort this image if the flag isundistorted is true
*/
void VRimageUpdater::frameUpdateEvent()
{
    m_isUpdating.lock();
    m_mutex.lock();
    if(this->m_camera->isGrabbing())
    {
        bool ret;
        QImage *qImage;

        qImage = this->m_camera->grab_image(ret);

        if(ret)
        {
            if(this->m_isUndistorted && this->m_camera->getIsinitUndistort())
                //Undistort Image
                m_frame = this->m_camera->undistortMapImage(*qImage,CV_INTER_LINEAR);
            else
                //NOT undistort
                m_frame = qImage->copy(qImage->rect());

            delete[] qImage->bits();
            delete qImage;
            qImage = NULL;

            m_currentFps = 1000.0/(this->m_crono.restart());
        }
    }
    m_mutex.unlock();
    m_isUpdating.unlock();
}

void VRimageUpdater::waitUpdateFinished()
{
    while(this->m_timeTrigger.isActive())
    {
        m_isUpdating.lock();
        m_isUpdating.unlock();
    }
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
 * function that starts or stops updating the image
 *
 * isUndistorted : True -> the image will be captured
 *                 False -> the image won't be captured
*/
void VRimageUpdater::setUpdatingEvent(bool updating)
{
    if(updating)
    {
        this->m_camera->startGrabbing(GrabStrategy_LatestImageOnly);
        this->m_timeTrigger.start();
    }
    else
    {
        m_mutex.lock();
        this->m_timeTrigger.stop();
        this->m_camera->stopGrabbing();
        m_mutex.unlock();
    }
}











