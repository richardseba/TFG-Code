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
