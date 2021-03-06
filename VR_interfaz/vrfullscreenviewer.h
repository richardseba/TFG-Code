#ifndef VRFULLSCREENVIEWER_H
#define VRFULLSCREENVIEWER_H

#include "camera.h"
#include "./VR_UI_Src/vrui.h"
#include "./VR_UI_Src/qgraphicstextitemvr.h"
#include "roitransition.h"
#include "depthprocessing.h"
#include "imageGeneratorSrc/imagegenerator.h"

#include <QTimer>
#include <QTime>
#include <QKeyEvent>
#include <QPalette>
#include <QApplication>
#include <QDesktopWidget>

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

const int STEPS_IN_TRANSITION = 10;

enum ViewingMode {NONE, VIDEO, CAMERA, STILL_IMG};

/* Class VrFullscreenViewer
 * -------------------------------
 * This class represents a fullscreen vr viewer that updated the scene using images captured
 * by a ImageGenerator. By default using a CameraImageGenerator.
 *
 * This class updates the scene in the function frameUpdateEvent each time the event from the
 * QTimer m_timer is fired ( is set by default to 0, hence firing again each time the frameUpdateEvent
 * is exited)
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
    VrFullscreenViewer(Camera* cameraL,Camera* cameraR,StereoCalibration stereoCalib);

public slots:
    void showFullScreen(int screenSelector);

private slots:
    void frameUpdateEvent();

private:
    void initScene();
    void saveUserParameters(QString filename,QString nameSufix="");
    void loadUserParameters(QString filename,bool transition=false);
    void setUpCameraVisualization(Camera* camL, Camera* camR);
    void setUpStillImages(char* nameFileL, char* nameFileR);
    void setUpVideo(char* nameFileL, char* nameFileR);
    void zoomIn();
    void zoomOut();
    int m_currentUserParam;

    Camera* m_cameraL;
    Camera* m_cameraR;

    ImageGenerator* m_imgGeneratorL;
    ImageGenerator* m_imgGeneratorR;

    QTime crono; //use to perform test, not necessary
    QTimer* m_timer;

    DepthProcessing* m_depthProcess;
    bool m_isProcessing;

    QTimer m_timerDepthProcess;

    QGraphicsScene m_scene;

    //Images that will be put on the scene
    QGraphicsPixmapItem m_frameR;
    QGraphicsPixmapItem m_frameL;

    Distance m_currentDistance;

    bool m_useUndistort;

    //User Interface
    QGraphicsTextItemVR* m_fpsCounter;

    float m_mean;

    Rect m_leftSensorROI;
    Rect m_rightSensorROI;

    ROITransition m_transitionLeft;
    ROITransition m_transitionRight;
    bool m_doTransitions;

    ViewingMode m_mode;
protected:
    void keyPressEvent(QKeyEvent *event);
};



#endif // VRFULLSCREENVIEWER_H
