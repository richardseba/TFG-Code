#ifndef VIDEOIMAGEGENERATOR_H
#define VIDEOIMAGEGENERATOR_H

#include <QImage>
#include <QTime>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "utils.h"
#include "imageGeneratorSrc/imagegenerator.h"

using namespace cv;


/* Class VideoImageGenerator
 * -------------------------------
 * This class is a image generator that grabs images from a video and stores the last frame in a local variable
 * that can be accesed using the getframe function
 *
 * This class loops over the process function, where it grabs the images. This function implements the super's
 * function from the threadedloopsevents.
 *
 * This class modifies the firing time between each call to the process() function to try to mantain the framerate
 * of the video
 *
 * The m_imageMutex is used to protect the acess over the m_currentFrame
 *
 * it inherits from ImageGenerator
*/
class VideoImageGenerator : public ImageGenerator
{
public:
    VideoImageGenerator();
    VideoImageGenerator(char* nameFile);
    ~VideoImageGenerator();

    QImage getFrame();
    int hasEnded();
protected:
    void process();

private:
    VideoCapture m_video;
    int m_expectedTimeInterFrames;

    QImage m_currentFrame;
    QMutex m_imageMutex;
};

#endif // VIDEOIMAGEGENERATOR_H
