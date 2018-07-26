#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QTime>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "utils.h"


class VideoPlayer : public QObject
{
    Q_OBJECT
public:
    VideoPlayer();
    ~VideoPlayer();
    VideoPlayer(char* namefile);
    QImage getFrame();
    void waitEndGrabEvent();
    bool isVideoFinished();
    void start();
    void stop();

private slots:
    void frameGrabEvent();
    void startEvent();
    void stopEvent();

signals:
    void startSignal();
    void stopSignal();

private:

    QTime m_crono;
    int m_expectedTimeInterFrames;

    QImage m_currentFrame;

    QMutex m_mutex;

    QThread m_thread;
    QTimer m_timeTrigger;

    VideoCapture m_video;
};

#endif // VIDEOPLAYER_H
