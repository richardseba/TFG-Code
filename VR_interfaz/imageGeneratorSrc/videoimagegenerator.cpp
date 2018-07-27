#include "videoimagegenerator.h"

VideoImageGenerator::VideoImageGenerator() : ImageGenerator ()
{
}

VideoImageGenerator::VideoImageGenerator(char* nameFile) : ImageGenerator ()
{
    m_video = VideoCapture(nameFile, cv::CAP_FFMPEG );
    qDebug() << "video opened:" <<  m_video.isOpened();
    int fps = (int) m_video.get(CAP_PROP_FPS);
    m_expectedTimeInterFrames = 1000/fps;
}

VideoImageGenerator::~VideoImageGenerator()
{
    this->stop();
    m_video.release();
}

QImage VideoImageGenerator::getFrame()
{
    QImage out;
    m_imageMutex.lock();
    out = m_currentFrame.copy();
    m_imageMutex.unlock();
    return out;
}

int  VideoImageGenerator::hasEnded()
{
    double framePos = m_video.get(CAP_PROP_POS_AVI_RATIO );
    return framePos >= 1;
}

void VideoImageGenerator::process()
{
    QTime tempcrono; tempcrono.start();

    Mat image;

    m_video >> image;

    if(!image.empty() && !hasEnded()){
         m_imageMutex.lock();
         m_currentFrame = Mat2QImage(image);
         m_imageMutex.unlock();
    }

    if(hasEnded())
    {
        this->stop();
    }
    m_timeTrigger.setInterval(max(m_expectedTimeInterFrames-tempcrono.restart(),0));
}


