#ifndef VRFULLSCREENVIEWER_H
#define VRFULLSCREENVIEWER_H

#include "camera.h"
#include "vrimageupdater.h"
#include <vrui.h>
#include "qgraphicstextitemvr.h"

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
#include <QPointF>
#include <QVector>
#include <QPolygonF>
#include <QGraphicsPolygonItem>

#include "QTime"
//#include <math.h>


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
    void zoomIn(float zoom);
    void zoomOut(float zoom);
private:
    void initScene();

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

    bool m_useUndistort;
    int m_width;
    int m_height;

    //User Interface
    //VrUI* m_interface;
    QGraphicsTextItemVR* m_fpsCounter;
    //QGraphicsRectItem* m_rectangle;
    float m_mean;

    int m_offsetLeftX;
    int m_offsetLeftY;

    int m_offsetRightX;
    int m_offsetRightY;

    int m_screenWidth;
    int m_screenHeight;

    // int m_offsetLeftCamera;
    // int m_offsetRightCamera;

protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif // VRFULLSCREENVIEWER_H
