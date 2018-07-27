#include "threadedloopsevents.h"

ThreadedLoopsEvents::ThreadedLoopsEvents()
{
    m_timeTrigger.moveToThread(&m_thread);
    connect(&m_timeTrigger, SIGNAL (timeout()), this, SLOT (frameLoopEvent()));
    connect(this, SIGNAL (startSignal()), this, SLOT (startEvent()));
    connect(this, SIGNAL (stopSignal()), this, SLOT (stopEvent()));
    this->moveToThread(&m_thread);
}

ThreadedLoopsEvents::~ThreadedLoopsEvents()
{
    this->stop();
    if(m_timeTrigger.isActive())
    {
        this->waitEndLoopEvent();
    }
    this->m_thread.quit();
    this->m_thread.wait();
}

void ThreadedLoopsEvents::waitEndLoopEvent()
{
    while(this->m_timeTrigger.isActive())
    {
        m_mutex.lock();
        m_mutex.unlock();
    }
}

void ThreadedLoopsEvents::start()
{
    m_thread.start();
    emit startSignal();
}

void ThreadedLoopsEvents::startEvent()
{
    if(!m_timeTrigger.isActive())
    {
        m_timeTrigger.start();
    }
}

void ThreadedLoopsEvents::stop()
{
    emit stopSignal();
}

void ThreadedLoopsEvents::stopEvent()
{
    m_mutex.lock();
    m_timeTrigger.stop();
    m_mutex.unlock();
}

void ThreadedLoopsEvents::frameLoopEvent()
{
    m_mutex.lock();
    process();
    m_mutex.unlock();
}

//void ThreadedLoopsEvents::process()
//{
//    //proces data in inherited classes
//    qDebug() << "spam";
//}








