#ifndef PRESETCLASSIFICATOR_H
#define PRESETCLASSIFICATOR_H

#include "utils.h"

enum Distance {CLOSE,MEDIUM,FAR};

class PresetClassificator
{
public:
    PresetClassificator(){}
    virtual Distance calcClasificationProximity(Mat imgL, Mat imgR) = 0;
    virtual float calcClasificationFloat(Mat imgL, Mat imgR) = 0;
    virtual void saveClasificator(char* namefile) = 0;
};

#endif // PRESETCLASSIFICATOR_H
