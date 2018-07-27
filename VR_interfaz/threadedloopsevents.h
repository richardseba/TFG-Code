#ifndef THREADEDLOOPSEVENTS_H
#define THREADEDLOOPSEVENTS_H

#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QDebug>

/* Class ABSTRACT ThreadedLoopsEvents
 * ------------------------------------------
 * This abstract class is designed to be used in enviaroments where a independent thread that
 * loops over one function is needed.
 *
 * The process function has to be implemented by the inherited childs
 * Start, stop and waitEndLoopEvent are implemented but can be overridden if needed
 *
 * The looping is done using a QTime event with start set at 0 by default (this means that
 * the event will fire inmediatly after exiting the previous QTime event)
 *
 * m_mutex is used in the thread critical parts, like the elimination of the thread itself
 * or the firing of the m_timeTrigger event.
*/
class ThreadedLoopsEvents : public QObject
{
    Q_OBJECT
public:
    ThreadedLoopsEvents();
    ~ThreadedLoopsEvents();

    virtual void start();
    virtual void stop();
    virtual void waitEndLoopEvent();

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
