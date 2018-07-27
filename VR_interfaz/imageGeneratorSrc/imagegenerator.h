#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#include "threadedloopsevents.h"

class ImageGenerator : public ThreadedLoopsEvents
{
public:
    ImageGenerator();
    ~ImageGenerator();
    virtual QImage getFrame() = 0;
    virtual int hasEnded() = 0;
};

#endif // IMAGEGENERATOR_H
