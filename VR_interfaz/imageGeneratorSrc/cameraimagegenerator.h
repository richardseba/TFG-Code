#ifndef CAMERAIMAGEGENERATOR_H
#define CAMERAIMAGEGENERATOR_H


#include <QTime>
#include <QMutex>

#include "imageGeneratorSrc/imagegenerator.h"
#include "camera.h"

/* Class CameraImageGenerator
 * -------------------------------
 * This class is a image generator that grabs images from a camera and stores the last frame in a local variable
 * that can be accesed using the getframe function
 *
 * This class loops over the process function, where it grabs the images. This function implements the super's
 * function from the threadedloopsevents.
 *
 * The m_imageMutex is used to protect the acess over the m_currentFrame
 *
 * it inherits from ImageGenerator
*/
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
