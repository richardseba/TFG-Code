#ifndef VRIMAGEUPDATER_H
#define VRIMAGEUPDATER_H

#include "QGraphicsItem"
#include "QTimer"
#include "QTime"
#include "QThread"
#include "camera.h"
#include "QDebug"

/* Class VRimageUpdater
 * -------------------------------
 * This class is used to capture frames of a Camera Object,
 * and save the most recent image captured.
 * It also can calculate the current fps (frames per second)
 * of the camera
 *
 * it inherits from QGraphicsView
 * Uses Q_OBJECT macro
*/
class VRimageUpdater : public QObject
{
    Q_OBJECT
public:
    VRimageUpdater();
    ~VRimageUpdater();
    VRimageUpdater(Camera* camera, QTimer* timer, bool mirrored,bool isUndistorted);
    QImage getNextFrame();
    void setIsUndistorted(bool isUndistorted);
    float getCurrentFPS();
public slots:
    void setUpdatingEvent(bool updating);
    void zoomIn(float zoom);
    void zoomOut(float zoom);
private slots:
    void frameUpdateEvent2();
private:
    bool m_mirrored;
    bool m_isUndistorted;

    QTimer* m_timeTrigger;
    QTime m_crono; //used for debugging reasons only
    Camera* m_camera;
    QImage m_frame;

    float m_currentFps;

    QMutex  m_mutex;
};

#endif // VRIMAGEUPDATER_H
