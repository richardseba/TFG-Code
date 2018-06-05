#ifndef VRFULLSCREENVIEWER_H
#define VRFULLSCREENVIEWER_H

#include "camera.h"
#include "vrimageupdater.h"
#include <vrui.h>
#include "qgraphicstextitemvr.h"
#include "roitransition.h"

#include "QTimer"
#include <QTime>
#include <QKeyEvent>
#include <QPalette>
#include <QApplication>
#include <QDesktopWidget>
#include <QThread>

#include <QGraphicsView>
#include "QGraphicsScene"
#include "QGraphicsItem"
#include "QGraphicsRectItem"
#include <QGraphicsLineItem>
#include <QPointF>
#include <QVector>
#include <QPolygonF>
#include <QGraphicsPolygonItem>

#include "QTime"


/* Class VrFullscreenViewer
 * -------------------------------
 * This class represents the fullscreen vr viewer that will
 * be used to display the images captured by vrimageupdater
 *
 * it inherits from QGraphicsView
 * Uses Q_OBJECT macro
*/
class VrFullscreenViewer : public QGraphicsView
{
    Q_OBJECT
public:
    VrFullscreenViewer();
    ~VrFullscreenViewer();
    VrFullscreenViewer(Camera* cameraL,Camera* cameraR);
public slots:
    void showFullScreen(int screenSelector);
private slots:
    void frameUpdateEvent();
signals:
    void setUpdatingR(bool update);
    void setUpdatingL(bool update);
private:
    void initScene();
    void saveUserParameters(QString filename,QString nameSufix="");
    void loadUserParameters(QString filename,bool transition=false);
    void zoomIn();
    void zoomOut();
    int m_currentUserParam;


    Camera* m_cameraL;
    Camera* m_cameraR;

    QTime crono; //use to perform test, not necessary
    QTimer* m_timer;

    //Timers for the update events in the threads
    QTimer m_timeR;
    QTimer m_timeL;

    VRimageUpdater* imageUpdaterR;
    VRimageUpdater* imageUpdaterL;

    QGraphicsScene m_scene;

    //Images that will be put on the scene
    QGraphicsPixmapItem m_frameR;
    QGraphicsPixmapItem m_frameL;

    //Threads used to grab the images from the cameras
    QThread m_threadR;
    QThread m_threadL;

    //demo
    QImage m_imgL;
    QImage m_imgR;
    int m_currentImage;
    bool m_isDemo;

    bool m_useUndistort;

    //User Interface
    QGraphicsTextItemVR* m_fpsCounter;
    QGraphicsLineItem m_splitLine;
    float m_mean;

    Rect m_leftSensorROI;
    Rect m_rightSensorROI;

    ROITransition m_transitionLeft;
    ROITransition m_transitionRight;
    bool m_doTransitions;

    //video demo
    VideoCapture* m_videoR;
    VideoCapture* m_videoL;
    bool m_isPlayingVideo;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // VRFULLSCREENVIEWER_H
