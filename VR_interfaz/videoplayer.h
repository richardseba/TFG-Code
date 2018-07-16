#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QThread>
#include <QTimer>
#include <QMutex>

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
    VideoPlayer(char* namefileL, char* namefileR);
    QImagePair getFrames();
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

    QImagePair m_currentFrames;

    QMutex m_mutex;

    QThread m_thread;
    QTimer m_timeTrigger;

    VideoCapture m_videoLeft;
    VideoCapture m_videoRight;
};

#endif // VIDEOPLAYER_H
