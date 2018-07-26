#ifndef THREADEDLOOPSEVENTS_H
#define THREADEDLOOPSEVENTS_H

#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QDebug>

class ThreadedLoopsEvents : public QObject
{
    Q_OBJECT
public:
    ThreadedLoopsEvents();
    ~ThreadedLoopsEvents();

    void start();
    void stop();
    void waitEndLoopEvent();

protected:
    virtual void process() = 0;

    QTimer m_timeTrigger;
    QMutex m_mutex;
    QThread m_thread;

private slots:
    void frameLoopEvent();
    void startEvent();
    void stopEvent();

signals:
    void startSignal();
    void stopSignal();

};

#endif // THREADEDLOOPSEVENTS_H
