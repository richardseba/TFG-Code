#include "cameraimagegenerator.h"

CameraImageGenerator::CameraImageGenerator() : ImageGenerator ()
{
}

CameraImageGenerator::CameraImageGenerator(Camera* camera, bool isUndistorted) : ImageGenerator ()
{
    this->m_isUndistorted = isUndistorted;
    this->m_camera = camera;
}

CameraImageGenerator::~CameraImageGenerator()
{
    this->stop();
    this->waitEndLoopEvent();
}

void CameraImageGenerator::process()
{
    if(this->m_camera->isGrabbing())
    {
        bool ret;
        QImage *qImage;

        qImage = this->m_camera->grab_image(ret);
        if(ret)
        {
            m_imageMutex.lock();
            if(this->m_isUndistorted && this->m_camera->getIsinitUndistort())
                //Undistort Image
                m_currentFrame = this->m_camera->undistortMapImage(*qImage,CV_INTER_LINEAR);
            else
                //NOT undistort
                m_currentFrame = qImage->copy(qImage->rect());

            m_imageMutex.unlock();

            delete[] qImage->bits();
            delete qImage;
            qImage = NULL;

            m_currentFps = 1000.0/(this->m_crono.restart());
        }
    }
}

/* public Function getCurrentFPS
 * -------------------------------
 * function that will return the current fps
*/
float CameraImageGenerator::getCurrentFps()
{
    return this->m_currentFps;
}

/* public Function getFrame
 * -------------------------------
 * function that will return the most recent image
*/
QImage CameraImageGenerator::getFrame()
{
    QImage image;

    m_imageMutex.lock();
    image = m_currentFrame.copy();
    m_imageMutex.unlock();

    return image;
}

/* public Function setIsUndistorted
 * -------------------------------
 * function that will set the undistorted flag
 *
 * isUndistorted : True -> the image will be undistorted
 *                 False -> the image won't be undistorted
*/
void CameraImageGenerator::setUndistortImages(bool undistort)
{
    this->m_isUndistorted = undistort;
}

int CameraImageGenerator::hasEnded()
{
    return 0;
}

void CameraImageGenerator::start()
{
    m_camera->startGrabbing();
    ImageGenerator::start();
}

void CameraImageGenerator::stop()
{
    ImageGenerator::stop();
    ImageGenerator::waitEndLoopEvent();
    m_camera->stopGrabbing();
}



















