#ifndef CAMERAIMAGEGENERATOR_H
#define CAMERAIMAGEGENERATOR_H


#include <QTime>
#include <QMutex>

#include "imageGeneratorSrc/imagegenerator.h"
#include "camera.h"


//TODO: pass the undistortion operation to an independent
//      imageprocessor
class CameraImageGenerator : public ImageGenerator
{
public:
    CameraImageGenerator();
    CameraImageGenerator(Camera* camera, bool isUndistorted = false);
    ~CameraImageGenerator();

    QImage getFrame();
    int hasEnded();
    void setUndistortImages(bool undistort);
    float getCurrentFps();

    void start();
    void stop();

protected:
    void process();

private:
    bool m_isUndistorted;
    QImage m_currentFrame;

    Camera* m_camera;

    QMutex m_imageMutex;

    float m_currentFps;
    QTime m_crono;
};

#endif // CAMERAIMAGEGENERATOR_H
