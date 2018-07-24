#include "videoplayer.h"

VideoPlayer::VideoPlayer(){}

VideoPlayer::VideoPlayer(char* namefile)
{
    m_video = VideoCapture(namefile, cv::CAP_FFMPEG );
    qDebug() << "video opened:" <<  m_video.isOpened();

    m_timeTrigger.moveToThread(&m_thread);
    connect(&m_timeTrigger, SIGNAL (timeout()), this, SLOT (frameGrabEvent()));
    connect(this, SIGNAL (startSignal()), this, SLOT (startEvent()));
    connect(this, SIGNAL (stopSignal()), this, SLOT (stopEvent()));
    this->moveToThread(&m_thread);
    m_thread.start();
}

VideoPlayer::~VideoPlayer()
{
    this->stop();
    if(m_timeTrigger.isActive())
    {
        this->waitEndGrabEvent();
    }
    m_video.release();
    this->m_thread.quit();
    this->m_thread.wait();
}

void VideoPlayer::waitEndGrabEvent()
{
    while(this->m_timeTrigger.isActive())
    {
        m_mutex.lock();
        m_mutex.unlock();
    }
}

void VideoPlayer::start()
{
    emit startSignal();
}

void VideoPlayer::startEvent()
{
    if(!m_timeTrigger.isActive())
    {
        int fps = (int)m_video.get(CAP_PROP_FPS);
//        int fps = 33;
        m_expectedTimeInterFrames = 1000/fps;
        m_timeTrigger.start(m_expectedTimeInterFrames);
    }
}

void VideoPlayer::stop()
{
    emit stopSignal();
}

void VideoPlayer::stopEvent()
{
    m_mutex.lock();
    m_timeTrigger.stop();
    m_mutex.unlock();
}

bool VideoPlayer::isVideoFinished()
{
    double framePos = m_video.get(CAP_PROP_POS_AVI_RATIO );
    return framePos >= 1;
//    return false;
}

void VideoPlayer::frameGrabEvent()
{
   QTime tempcrono; tempcrono.start();

   Mat image;

   m_video >> image;

   if(!image.empty() && !isVideoFinished()){
        m_mutex.lock();
//        QTime asd; asd.restart();
        m_currentFrame = Mat2QImage(image);
//        qDebug() << asd.restart();
        m_mutex.unlock();
   }

   if(isVideoFinished())
   {
       this->stop();
   }
   m_timeTrigger.setInterval(max(m_expectedTimeInterFrames-tempcrono.restart(),0));

//   qDebug() << "Total Time" << m_crono.restart();
}

QImage VideoPlayer::getFrame()
{
    QImage out;
    m_mutex.lock();
    out = m_currentFrame.copy();
    m_mutex.unlock();
    return out;
}
