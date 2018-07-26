#include "imageloadergenerator.h"

ImageLoaderGenerator::ImageLoaderGenerator() : ImageGenerator ()
{
}

ImageLoaderGenerator::ImageLoaderGenerator(char* nameFile) : ImageGenerator ()
{
    m_image = QImage(nameFile);
}

QImage ImageLoaderGenerator::getFrame()
{
    return m_image;
}

void ImageLoaderGenerator::process()
{
}

void ImageLoaderGenerator::start()
{
}

void ImageLoaderGenerator::stop()
{
}

int ImageLoaderGenerator::hasEnded()
{
    return 1;
}

