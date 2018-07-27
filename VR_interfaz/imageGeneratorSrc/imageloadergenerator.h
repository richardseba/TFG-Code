#ifndef IMAGELOADERGENERATOR_H
#define IMAGELOADERGENERATOR_H

#include <QImage>
#include "imageGeneratorSrc/imagegenerator.h"


class ImageLoaderGenerator : public ImageGenerator
{
public:
    ImageLoaderGenerator(char* nameFile);
    ~ImageLoaderGenerator();

    QImage getFrame();
    int hasEnded();

    void start();
    void stop();

protected:
    void process();

private:
    ImageLoaderGenerator();
    QImage m_image;
};

#endif // IMAGELOADERGENERATOR_H
