#ifndef IMAGELOADERGENERATOR_H
#define IMAGELOADERGENERATOR_H

#include <QImage>
#include "imageGeneratorSrc/imagegenerator.h"

/* Class ImageLoaderGenerator
 * -------------------------------
 * This class is a image generator that loads one image from a image file and stores it  on the local variable
 * that can be accesed using the getframe function
 *
 * This class overrides the looping functionality, as only one load is necesary to load the images
 *
 * it inherits from ImageGenerator
*/
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
