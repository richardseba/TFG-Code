#ifndef IRIMAGEGENERATOR_H
#define IRIMAGEGENERATOR_H

#include "imageGeneratorSrc/imagegenerator.h"
#include <QImage>
#include <QMutex>

class IRimageGenerator : public ImageGenerator
{
public:
    IRimageGenerator();
    IRimageGenerator(ImageGenerator* imgGen);

    void start();
    void stop();

    QImage getFrame();
    int hasEnded();

protected:
    void process();

private:
    ImageGenerator* m_imgGen;
    QImage m_currentFrame;
    QMutex m_imageMutex;
};

#endif // IRIMAGEGENERATOR_H
