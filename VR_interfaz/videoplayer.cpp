#include "videoplayer.h"

VideoPlayer::VideoPlayer(){}

VideoPlayer::VideoPlayer(char* namefileL, char* namefileR)
{
    m_videoLeft = VideoCapture(namefileL);
    m_videoRight = VideoCapture(namefileR);

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
    m_videoLeft.release();
    m_videoRight.release();
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
        int fps = (int)m_videoLeft.get(CAP_PROP_FPS);

//        m_timeTrigger.start(1000/fps);
        m_timeTrigger.start();
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
    bool isFinished = false;
    double framePosL = m_videoLeft.get(CAP_PROP_POS_AVI_RATIO  );
    double framePosR = m_videoRight.get(CAP_PROP_POS_AVI_RATIO  );

    if( max(framePosL,framePosR) >= 1) isFinished = true;
    return isFinished;
}

void VideoPlayer::frameGrabEvent()
{
   QTime tempcrono; tempcrono.start();

   Mat imageL, imageR;

   m_videoLeft >> imageL;
   m_videoRight >> imageR;
   if(!imageL.empty() && !imageR.empty() && !isVideoFinished()){
        m_mutex.lock();
        m_currentFrames.l = Mat2QImage(imageL);
        m_currentFrames.r = Mat2QImage(imageR);
        m_mutex.unlock();
   }

   if(isVideoFinished())
   {
       this->stop();
   }

   qDebug() << "Total Time" << m_crono.restart() << "process" << tempcrono.restart() ;
}

QImagePair VideoPlayer::getFrames()
{
    QImagePair out;

    m_mutex.lock();
    out.l = m_currentFrames.l.copy();
    out.r = m_currentFrames.r.copy();
    m_mutex.unlock();

    return out;
}
