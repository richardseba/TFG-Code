#ifndef IMAGEGENERATOR_H
#define IMAGEGENERATOR_H

#include "threadedloopsevents.h"
#include <QImage>
#include "iostream"

/* Class ABSTRACT ImageGenerator
 * -------------------------------
 * This ABSTRACT class contains the virtual functions that a Image generator has to have
 *
 * Both getFrame() and hasEnded() have to be implemented by the child classes
 *
 * it inherits from ThreadedLoopsEvents
*/
class ImageGenerator : public ThreadedLoopsEvents
{
public:
    ImageGenerator();
    ~ImageGenerator();
    virtual QImage getFrame() = 0;
    virtual std::tuple<QImage, unsigned long> getFrameWithID() { std::cout << "asd\n"; return std::make_tuple(QImage(),0);}
    virtual int hasEnded() = 0;
protected:
    unsigned long m_currentframeID = 0;
};

#endif // IMAGEGENERATOR_H
